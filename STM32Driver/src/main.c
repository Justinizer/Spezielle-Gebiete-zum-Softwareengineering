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


int daemonizeFlag = 0;
int getDataFlag = 0;
int printDataRawFlag = 0;
int setBrightnessFlag = 0;
int setBrightness = 0;
int publishFlag = 0;

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
static int send_data(const char *data);
static void parse_data_string(const char *data, float *pm2_5, float *pm10, float *temperature, float *humidity);

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
		send_data(buffer);
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

static int send_data(const char *data) {
	MQTTClient client;
	MQTTClient_connectOptions opt = MQTTClient_connectOptions_initializer;
	MQTTClient_message msg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	char valueBuffer[10];
	int result;

	const char *BROKER_ADDRESS = "tcp://broker.hivemq.com:1883";
	const char *CLIENT_ID = "stm32driver";
	const char *TEMPERATURE_TOPIC = "temperature";
	const char *HUMIDITY_TOPIC = "humidity";
	const char *PM2_5_TOPIC = "pm2_5";
	const char *PM10_TOPIC = "pm10";

	MQTTClient_create(&client, BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	opt.keepAliveInterval = 20;
	opt.cleansession = 1;

	result = MQTTClient_connect(client, &opt);
	if (result != MQTTCLIENT_SUCCESS) {
		printf("Error connection to broker! Result: %d\n", result);
		return 1;
	}

	msg.qos = 1;
	msg.retained = 0;

	float pm2_5, pm10;
	float temperature;
	float humidity;


	parse_data_string(data, &pm2_5, &pm10, &temperature, &humidity);


	snprintf(valueBuffer, 10, "%.1f", pm2_5);
	msg.payload = valueBuffer;
	msg.payloadlen = strlen(valueBuffer);
	MQTTClient_publishMessage(client, PM2_5_TOPIC, &msg, &token);

	snprintf(valueBuffer, 10, "%.1f", pm10);
	msg.payload = valueBuffer;
	msg.payloadlen = strlen(valueBuffer);
	MQTTClient_publishMessage(client, PM10_TOPIC, &msg, &token);

	snprintf(valueBuffer, 10, "%.1f", temperature);
	msg.payload = valueBuffer;
	msg.payloadlen = strlen(valueBuffer);
	MQTTClient_publishMessage(client, TEMPERATURE_TOPIC, &msg, &token);

	snprintf(valueBuffer, 10, "%.1f", humidity);
	msg.payload = valueBuffer;
	msg.payloadlen = strlen(valueBuffer);
	MQTTClient_publishMessage(client, HUMIDITY_TOPIC, &msg, &token);


	result = MQTTClient_waitForCompletion(client, token, 10000);
	printf("Message was delivered. Token: %d\n", token);

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);

	return 0;
}

static void parse_data_string(const char *data, float *pm2_5, float *pm10, float *temperature, float *humidity) {
	if (!data || !pm2_5 || !pm10 || !temperature || !humidity) {
		return;
	}

	sscanf(data, "%f;%f;%f;%f", pm2_5, pm10, temperature, humidity);

	*pm2_5 = (*pm2_5) / 10.0f;
	*pm10 = (*pm10) / 10.0f;
	*temperature = (*temperature) / 10.0f;
	*humidity = (*humidity) / 10.0f;
}