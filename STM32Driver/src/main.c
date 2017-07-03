#define _GNU_SOURCE	1	// Include gnu c extensions to use strdup()
#include <string.h>		// For strerror(), strdup()
#include <stdlib.h>
#include <getopt.h>		// For getopt_long()
#include <unistd.h>		// For sleep()
#include <syslog.h>		// For openlog(), syslog()
#include <stdio.h>
#include <errno.h>		// For errno
#include <stdint.h>
#include "daemonize.h"
#include "commands.h"
#include "serial.h"
#include "mqtt.h"

#define RECEIVE_BUFFER_SIZE	100
#define PUBLISH_TIME		60

typedef struct {
	const char *topic;			// Name of the topic to which the value should be published.
	const char *value_name;		// Name of the value for output.
	const char *unit;			// Unit of the value for output.
	float factor;				// Factor with which the value is multiplied.
	uint8_t value_is_float;		// Is the read value a floataing point value? (Used for output)
} sensor_t;

static const sensor_t SENSORS[] = {
//	topic						value_name			unit		factor		value_is_float
	{"/garten/pm2_5",			"Feinstaub 2,5µm",	"µg/m³",	.1f,			1},
	{"/garten/pm10",			"Feinstaub 10µm",	"µg/m³",	.1f,			1},
	{"/wohnzimmer/temperatur",	"Temperatur",		"°C",		.1f,			1},
	{"/wohnzimmer/feuchtigkeit","Luftfeuchte",		"%rF",		.1f, 			1},
	{"/wohnzimmer/bodenfeuchte","Bodenfeuchte",		NULL,		1.0f, 			0},
	{"/wohnzimmer/helligkeit",	"Helligkeit",		NULL,		1.0f,				0},
	{"/wohnzimmer/mic",			"Lautstärke",		"dBa",		126.8f,			1},
	{"/wohnzimmer/bewegung",	"Bewegung",			NULL,		1.0f,				0},
	{"/kueche/voc",				"VOC",				"MOF",		1.0f,				0},
	{"/kueche/feuer",			"Feuer",			NULL,		1.0f,				0}
};

static const size_t SENSOR_SIZE = sizeof(SENSORS) / sizeof(SENSORS[0]);
static const char *DEFAULT_BROKER_ADDRESS = "tcp://broker.hivemq.com:1883";
const char *serial0 = "/dev/ttyAMA0";
const char *serial1 = "/dev/ttyACM0";
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

	// Switch to default broker address if there wasn't any address provided.
	if (!broker_address) {
		broker_address = (char *)DEFAULT_BROKER_ADDRESS;
	}

	if (getDataFlag || publishFlag) {
		return get_and_print_data(publishFlag, 1, printDataRawFlag);
	}

	if (setBrightnessFlag) {
		return set_brightness(serial0, setBrightness);
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

			sleep(PUBLISH_TIME);
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
	size_t data1_len;
	float value;
	int availability;
	int result;

	availability = get_value_availability(serial0);
	if (availability == -1) {
		return 1;
	}

	if (availability == 0) {
		if (daemonizeFlag) {
			syslog(LOG_INFO, "No values available.");

		} else {
			printf("No values available.\n");
		}

		return 0;
	}

	if (get_data(serial0, buffer, RECEIVE_BUFFER_SIZE) == 1) {
		return 1;
	}

	data1_len = strlen(buffer);
	buffer[data1_len] = ';';

	if (get_data(serial1, buffer + data1_len + 1, RECEIVE_BUFFER_SIZE - data1_len - 1) == 1) {
		return 1;
	}

	if (print && printRaw) {
		printf("%s\n", buffer);
	} 


	if (publish && !daemonizeFlag) {
		result = open_mqtt_connection(&client, broker_address, CLIENT_ID);

		if (result == 1) {
			return 1;
		}
	}


	currentValuePtr = strtok(buffer, ";");
	while (currentValuePtr && index < SENSOR_SIZE) {

		sscanf(currentValuePtr, "%f", &value);

		value *= SENSORS[index].factor;

		if (print && !printRaw) {
			unit = (SENSORS[index].unit) ? SENSORS[index].unit : "";
			printf("%s: %4.*f %s\n", SENSORS[index].value_name, SENSORS[index].value_is_float, value, unit);
		}

		if (publish) {
			if (mqtt_send_value(&client, SENSORS[index].topic, value) == 1) {
				break;
			}
		}

		currentValuePtr = strtok(NULL, ";");
		index++;
	}

	if (publish && !daemonizeFlag) {
		close_mqtt_connection(&client);
	}

	return 0;
}
