#define _GNU_SOURCE	1	// Include gnu c extensions to use strdup()
#include <string.h>		// For strerror(), strdup()
#include <stdlib.h>
#include <getopt.h>		// For getopt_long()
#include <unistd.h>		// For sleep()
#include <syslog.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include "daemonize.h"
#include "commands.h"
#include "serial.h"
#include "mqtt.h"

#define RECEIVE_BUFFER_SIZE	20

typedef struct {
	const char *topic;
	const char *value_name;
	const char *unit;
	uint8_t div_by_10;
	uint8_t value_is_float;
} sensor_t;

static const sensor_t SENSORS[] = {
//	topic						value_name			unit		div_by_10	value_is_float
	{"/garten/pm2_5",			"Feinstaub 2,5µm",	"µg/m³",	1,			1},
	{"/garten/pm10",			"Feinstaub 10µm",	"µg/m³",	1,			1},
	{"/wohnzimmer/temperatur",	"Temperatur",		"°C",		1,			1},
	{"/wohnzimmer/feuchtigkeit","Luftfeuchte",		"%rF",		1, 			1},
	{"/wohnzimmer/bodenfeuchte","Bodenfeuchte",		NULL,		0, 			0}
};

static const size_t SENSOR_SIZE = sizeof(SENSORS) / sizeof(SENSORS[0]);
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
	{"get-data",		no_argument, 		NULL,	'g'},
	{"help",			no_argument, 		NULL,	'h'},
	{"publish",			no_argument,		NULL,	'p'},
	{"raw",				no_argument, 		NULL,	'r'},
	{"set-brightness",	required_argument,	NULL,	's'},
	{0, 0, 0, 0}
};

static void print_help(const char *progname);
static int get_and_print_data(int publish, int print, int printRaw);

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
		return get_and_print_data(publishFlag, 1, printDataRawFlag);
	}

	if (setBrightnessFlag) {
		return set_brightness(setBrightness);
	}

	if (daemonizeFlag) {
		printf("Starting as daemon...\n");
		if (daemonize() == 1) {
			return 1;
		}

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
			get_and_print_data(1, 0, 0);
			syslog(LOG_INFO, "Publishing data.");

			sleep(60);	// Wait 5 mins
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

static int get_and_print_data(int publish, int print, int printRaw) {
	char *currentValuePtr;
	const char *unit;
	size_t index = 0;
	char buffer[RECEIVE_BUFFER_SIZE];
	float value;

	if (get_data(buffer, RECEIVE_BUFFER_SIZE) == 1) {
		return 1;
	}

	if (print && printRaw) {
		printf("%s\n", buffer);

	} 

	if (publish) {
		open_mqtt_connection(&client, broker_address, CLIENT_ID);
	}


	currentValuePtr = strtok(buffer, ";");
	while (currentValuePtr && index < SENSOR_SIZE) {

		sscanf(currentValuePtr, "%f", &value);

		if (SENSORS[index].div_by_10) {
			value /= 10.0f;
		}

		if (print && !printRaw) {
			unit = (SENSORS[index].unit) ? SENSORS[index].unit : "";
			printf("%s: %4.*f %s\n", SENSORS[index].value_name, SENSORS[index].value_is_float, value, unit);
		}

		if (publish) {
			mqtt_send_value(client, SENSORS[index].topic, value);
		}

		currentValuePtr = strtok(NULL, ";");
		index++;
	}

	if (publish) {
		close_mqtt_connection(&client);
	}

	return 0;
}
