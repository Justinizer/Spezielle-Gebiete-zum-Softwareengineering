#include <string.h>		// For strerror()
#include <stdlib.h>
#include <getopt.h>		// For getopt_long()
#include <unistd.h>		// For sleep()
#include <stdio.h>
#include <errno.h>
#include "serial.h"
#include "daemonize.h"
#include <MQTTClient.h>

#define COMMAND_PACKET_HEADER	0xAA
#define COMMAND_PACKET_TAIL		0xFF
#define COMMAND_GET_DATA		0x01
#define COMMAND_SET_BRIGHTNESS	0x02

static const char *VALUE_NAMES[] = {"PM2_5", "PM10", "temperature", "humidity"};
static const char *BROKER_ADDRESS = "tcp://broker.hivemq.com:1883";
static const char *CLIENT_ID = "stm32driver";

int daemonizeFlag = 0;
int getDataFlag = 0;
int printDataRawFlag = 0;
int setBrightnessFlag = 0;
int setBrightness = 0;
int publishFlag = 0;

MQTTClient client;

static const struct option program_options[] = {
//	name				has_arg				flag	val
	{"daemonize",		no_argument, 		NULL,	'd'},
	{"get-data", 		no_argument, 		NULL,	'g'},
	{"help",			no_argument, 		NULL,	'h'},
	{"publish",			no_argument,		NULL,	'p'},
	{"raw",				no_argument, 		NULL,	'r'},
	{"set-brightness",	required_argument,	NULL,	's'},
	{0, 0, 0, 0}
};

static void print_help(const char *progname);
static int get_data(char *buffer, size_t buffersize);
static int get_and_print_data(int publish);
static int set_brightness(int brightness);
static int open_mqtt_connection(MQTTClient *client);
static int close_mqtt_connection(MQTTClient *client);
static int send_values(MQTTClient *client, const char *data);
static int send_value(MQTTClient *client, const char *name, float value);

int main(int argc, char *argv[]) {
	int optionIndex;
	int c;

	if (argc == 1) {
		print_help(argv[0]);
		return 0;
	}

	while ((c = getopt_long(argc, argv, "dghprs:", program_options, &optionIndex)) != -1) {

		switch (c) {
			case 'd':
				daemonizeFlag = 1;
				break;

			case 'g':
				getDataFlag = 1;
				break;

			case 'h':
			default:
				print_help(argv[0]);
				return 0;

			case 'p':
				publishFlag = 1;
				break;

			case 'r':
				printDataRawFlag = 1;
				break;

			case 's':
				setBrightnessFlag = 1;
				setBrightness = atoi(optarg);
				break;
		}
	}

	if (getDataFlag || publishFlag) {
		return get_and_print_data(publishFlag);
	}

	if (setBrightnessFlag) {
		return set_brightness(setBrightness);
	}

	if (daemonizeFlag) {
		printf("Starting as daemon...\n");
		if (daemonize() == 1) {
			return 1;
		}

		char buffer[20];

		while (1) {
			get_data(buffer, 20);

			// TODO: Do smth. with data

			sleep(300);	// Wait 5 mins
		}
	}

	return 0;
}

static void print_help(const char *progname) {
	printf("Usage: %s [options]...\n\n", progname);
	printf("Available options:\n");
	printf("  -d, --daemonize\t\tStart program as a deamon\n");
	printf("  -g, --get-data\t\tGet and print data from all sensors then quit\n");
	printf("  -h, --help\t\t\tShow this help\n");
	printf("  -p, --publish\t\t\tGet, print and publish data then quit\n");
	printf("  -r, --raw\t\t\tPrint data raw\n");
	printf("  -s, --set-brightness <value>\tSet brightness to <value>\n\n");
}

static int get_data(char *buffer, size_t buffersize) {
	char packet[4] = {COMMAND_PACKET_HEADER, COMMAND_GET_DATA, 0x00, COMMAND_PACKET_TAIL};
	int fd;

	if (!buffer || buffersize == 0) {
		return 1;
	}

	fd = serial_open();
	if (fd == -1) {
		printf("Error opening the serial connection: %s\n", strerror(errno));
		return 1;
	}

	if (serial_write_buffer(fd, packet, 4) < 0) {
		printf("Error sending data: %s\n", strerror(errno));
		serial_close(fd);
		return 1;
	}

	serial_read_string(fd, buffer, buffersize);
	serial_close(fd);


	return 0;
}

static int get_and_print_data(int publish) {
	char buffer[20];

	if (get_data(buffer, 20) == 1) {
		return 1;
	}

	if (printDataRawFlag) {
		printf("%s\n", buffer);

	} else {
		float pm2_5, pm10;
		float temperature;
		float humidity;

		parse_data_string(buffer, &pm2_5, &pm10, &temperature, &humidity);

		printf("PM 2,5µm:    %4.1f µg/m³\n", pm2_5);
		printf("PM 10µm :    %4.1f µg/m³\n", pm10);
		printf("Temperature: %4.1f °C\n", temperature);
		printf("Humidity:    %4.1f %%rF\n", humidity);
	}

	if (publish) {
		open_mqtt_connection(&client);
		send_values(&client, buffer);
		close_mqtt_connection(&client);
	}

	return 0;
}

static int set_brightness(int brightness) {
	char packet[4] = {COMMAND_PACKET_HEADER, COMMAND_SET_BRIGHTNESS, 0x00, COMMAND_PACKET_TAIL};
	int fd;

	if (brightness < 0) {
		brightness = 0;
	}

	packet[2] = (brightness & 0xFF);

	fd = serial_open();
	if (fd == -1) {
		printf("Error opening the serial connection: %s\n", strerror(errno));
		return 1;
	}

	if (serial_write_buffer(fd, packet, 4) < 0) {
		printf("Error sending data: %s\n", strerror(errno));
		serial_close(fd);
		return 1;
	}

	serial_close(fd);

	return 0;
}

static int open_mqtt_connection(MQTTClient *client) {
	MQTTClient_connectOptions opt = MQTTClient_connectOptions_initializer;
	int result;

	if (!client) {
		return 1;
	}

	MQTTClient_create(client, BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	opt.keepAliveInterval = 20;
	opt.cleansession = 1;

	result = MQTTClient_connect(*client, &opt);
	if (result != MQTTCLIENT_SUCCESS) {
		printf("Error connecting to broker! Result: %d\n", result);
		return 1;
	}


	return 0;
}

static int close_mqtt_connection(MQTTClient *client) {
	if (!client) {
		return 1;
	}

	MQTTClient_disconnect(*client, 10000);
	MQTTClient_destroy(client);

	return 0;
}

static int send_values(MQTTClient *client, const char *data) {
	size_t valueStringLen;
	char *valueStringBuffer;
	char *currentValuePtr;
	size_t index = 0;
	float value;

	if (!client || !data) {
		return 1;
	}


	valueStringLen = strlen(data);
	valueStringBuffer = malloc(valueStringLen + 1);
	if (!valueStringBuffer) {
		return 1;
	}

	strcpy(valueStringBuffer, data);


	currentValuePtr = strtok(valueStringBuffer, ";");
	while (currentValuePtr) {

		sscanf(currentValuePtr, "%f", &value);
		value /= 10.0f;

		send_value(client, VALUE_NAMES[index], value);

		currentValuePtr = strtok(NULL, ";");
		index++;
	}


	free(valueStringBuffer);

	return 0;
}

static int send_value(MQTTClient *client, const char *topic, float value) {
	MQTTClient_message msg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	char valueBuffer[10];
	int result;

	if (!client || !topic) {
		return 1;
	}

	msg.qos = 1;
	msg.retained = 0;

	snprintf(valueBuffer, 10, "%.1f", value);
	msg.payload = valueBuffer;
	msg.payloadlen = strlen(valueBuffer);

	result = MQTTClient_publishMessage(*client, topic, &msg, &token);
	if (result != MQTTCLIENT_SUCCESS) {
		printf("Error publishing message. Result: %d\n", result);
		return 1;
	}

	result = MQTTClient_waitForCompletion(*client, token, 10000);
	if (result != MQTTCLIENT_SUCCESS) {
		printf("Error delivering message. Result: %d\n", result);
		return 1;
	}

	return 0;
}