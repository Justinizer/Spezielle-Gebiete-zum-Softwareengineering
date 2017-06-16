#define _GNU_SOURCE	1	// Include gnu c extensions to use strdup()
#include <string.h>		// For strerror(), strdup()
#include <stdlib.h>
#include <getopt.h>		// For getopt_long()
#include <unistd.h>		// For sleep()
#include <syslog.h>
#include <stdio.h>
#include <errno.h>
#include "serial.h"
#include "daemonize.h"
#include "commands.h"
#include "mqtt.h"

static const char *VALUE_NAMES[] = {"/garten/pm2_5", "/garten/pm10", "/wohnzimmer/temperatur", "/wohnzimmer/feuchtigkeit"};
static const int VALUE_NAMES_LEN = sizeof(VALUE_NAMES) / sizeof(VALUE_NAMES[0]);
static const char *DEFAULT_BROKER_ADDRESS = "tcp://broker.hivemq.com:1883";
static const char *CLIENT_ID = "stm32driver";
const char *BRIGHTNESS_TOPIC = "/wohnzimmer/led/rot";

int daemonizeFlag = 0;
int getDataFlag = 0;
int printDataRawFlag = 0;
int setBrightnessFlag = 0;
int setBrightness = 0;
int publishFlag = 0;

MQTTClient client;
char *broker_address = NULL;

static const struct option program_options[] = {
//	name				has_arg				flag	val
	{"broker-address",	required_argument,	NULL,	'b'},
	{"daemonize",		no_argument, 		NULL,	'd'},
	{"get-data", 		no_argument, 		NULL,	'g'},
	{"help",			no_argument, 		NULL,	'h'},
	{"publish",			no_argument,		NULL,	'p'},
	{"raw",				no_argument, 		NULL,	'r'},
	{"set-brightness",	required_argument,	NULL,	's'},
	{0, 0, 0, 0}
};

static void print_help(const char *progname);
static int get_and_print_data(int publish);
static int send_values(MQTTClient *client, const char *data);

int main(int argc, char *argv[]) {
	int optionIndex;
	int result;
	int c;

	if (argc == 1) {
		print_help(argv[0]);
		return 0;
	}

	while ((c = getopt_long(argc, argv, "b:dghprs:", program_options, &optionIndex)) != -1) {

		switch (c) {
			case 'b':
				broker_address = strdup(optarg);
				if (!broker_address) {
					printf("Error duplicating broker address: %s\n", strerror(errno));
					return 1;
				}
				break;

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

	if (!broker_address) {
		broker_address = (char *)DEFAULT_BROKER_ADDRESS;
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
		result = open_mqtt_connection(&client, broker_address, CLIENT_ID);
		if (result == 1) {
			syslog(LOG_ERR, "Error connecting to broker. Shutting down.");
			exit(1);

		} else {
			syslog(LOG_INFO, "Successfully connected to broker (%s)", broker_address);
		}

		result = MQTTClient_subscribe(client, BRIGHTNESS_TOPIC, 1);
		if (result != MQTTCLIENT_SUCCESS) {
			syslog(LOG_ERR, "Error subscribing to topic '%s'", BRIGHTNESS_TOPIC);
			exit(1);
		}


		while (1) {
			get_data(buffer, 20);
			send_values(&client, buffer);
			syslog(LOG_INFO, "Publishing data.");

			// TODO: Do smth. with data

			sleep(300);	// Wait 5 mins
		}
	}



	if (broker_address != DEFAULT_BROKER_ADDRESS) {
		free(broker_address);
	}

	return 0;
}

static void print_help(const char *progname) {
	printf("Usage: %s [options]...\n\n", progname);
	printf("Available options:\n");
	printf("  -b, --broker-address <address>  Set address of MQTT broker to <address>\n");
	printf("  -d, --daemonize                 Start program as a deamon\n");
	printf("  -g, --get-data                  Get and print data from all sensors then quit\n");
	printf("  -h, --help                      Show this help\n");
	printf("  -p, --publish                   Get, print and publish data then quit\n");
	printf("  -r, --raw                       Print data raw\n");
	printf("  -s, --set-brightness <value>    Set brightness to <value>\n\n");
}

static int get_and_print_data(int publish) {
	char buffer[20];

	if (get_data(buffer, 20) == 1) {
		return 1;
	}

	if (printDataRawFlag) {
		printf("%s\n", buffer);

	} /*else {
		float pm2_5, pm10;
		float temperature;
		float humidity;

		parse_data_string(buffer, &pm2_5, &pm10, &temperature, &humidity);

		printf("PM 2,5µm:    %4.1f µg/m³\n", pm2_5);
		printf("PM 10µm :    %4.1f µg/m³\n", pm10);
		printf("Temperature: %4.1f °C\n", temperature);
		printf("Humidity:    %4.1f %%rF\n", humidity);
	}*/

	if (publish) {
		open_mqtt_connection(&client, broker_address, CLIENT_ID);
		send_values(&client, buffer);
		close_mqtt_connection(&client);
	}

	return 0;
}

static int send_values(MQTTClient *client, const char *data) {
	char *valueStringBuffer;
	char *currentValuePtr;
	size_t index = 0;
	float value;

	if (!client || !data) {
		return 1;
	}


	valueStringBuffer = strdup(data);
	if (!valueStringBuffer) {
		printf("Error duplicating data string: %s\n", strerror(errno));
		return 1;
	}


	currentValuePtr = strtok(valueStringBuffer, ";");
	while (currentValuePtr && index < VALUE_NAMES_LEN) {

		sscanf(currentValuePtr, "%f", &value);
		value /= 10.0f;

		mqtt_send_value(client, VALUE_NAMES[index], value);

		currentValuePtr = strtok(NULL, ";");
		index++;
	}


	free(valueStringBuffer);

	return 0;
}

