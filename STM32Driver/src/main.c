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
#include "parson.h"
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


static const char *DEFAULT_BROKER_ADDRESS = "tcp://broker.hivemq.com:1883";
static const char *CLIENT_ID = "stm32driver";
static const char *CONFIG_FILE_PATH = "cfg/config.json";
const char *BRIGHTNESS_TOPIC = "/wohnzimmer/led/rot";
const char *serial0 = "/dev/ttyAMA0";
const char *serial1 = "/dev/ttyACM0";

sensor_t *sensors = NULL;
size_t sensors_size = 0;

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
static int load_sensor_configs();
static int read_values_from_sensors(char *buffer, size_t buffersize);
static int get_and_print_data(int publish, int print, int printRaw);

int main(int argc, char *argv[]) {
	int optionIndex;
	int result;
	int c;

	if (argc == 1) {
		print_help(argv[0]);
		return 0;
	}

	load_sensor_configs();

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
		if (result == 0) {
			syslog(LOG_INFO, "Successfully connected to broker (%s)", broker_address);

		} else {
			syslog(LOG_ERR, "Error connecting to broker. Shutting down.");
			exit(1);
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

static int load_sensor_configs() {
	JSON_Value *root;
	JSON_Object *obj;
	JSON_Array *sensor_array;
	char *broker_addr;

	root = json_parse_file(CONFIG_FILE_PATH);
	if (json_value_get_type(root) != JSONObject) {
		if (daemonizeFlag) {
			syslog(LOG_ERR, "Couldn't read config file!");

		} else {
			printf("Couldn't read config file!\n");
		}

		exit(1);
	}

	obj = json_value_get_object(root);
	broker_addr = (char *) json_object_get_string(obj, "broker_address");
	if (broker_addr) {
		broker_address = strdup(broker_addr);

	} else {
		broker_address = (char *) DEFAULT_BROKER_ADDRESS;
	}


	sensor_array = json_object_get_array(obj, "sensors");
	sensors = calloc(json_array_get_count(sensor_array), sizeof(sensor_t));
	if (!sensors) {
		if (daemonizeFlag) {
			syslog(LOG_ERR, "Error while allocating memory: %s", strerror(errno));
			
		} else {
			printf("Error while allocating memory: %s\n", strerror(errno));
		}

		exit(1);
	}


	sensors_size = json_array_get_count(sensor_array);
	for (int i = 0; i < sensors_size; i++) {
		JSON_Object *arr_obj = json_array_get_object(sensor_array, i);

		const char *topic = json_object_get_string(arr_obj, "topic");
		const char *value = json_object_get_string(arr_obj, "valuename");
		const char *unit = json_object_get_string(arr_obj, "unit");
		int is_float = json_object_get_boolean(arr_obj, "is_float");

		if (!topic || !value) {
			printf("topic or value at index %d not in json file!\n", i);
			json_value_free(root);
			exit(1);
		}

		sensors[i].topic = strdup(topic);
		sensors[i].value_name = strdup(value);
		sensors[i].unit = (unit) ? strdup(unit) : NULL;
		sensors[i].factor = json_object_get_number(arr_obj, "factor");
		sensors[i].value_is_float = (is_float == -1) ? 1 : is_float;

		printf("Topic: %s\n", json_object_get_string(arr_obj, "topic"));
	}


	printf("Client ID: %s\n", json_object_get_string(obj, "mqtt_client_id"));
	printf("Broker address: %s\n", broker_address);


	json_value_free(root);
	return 0;
}

static int read_values_from_sensors(char *buffer, size_t buffersize) {
	int availability;
	size_t data1_len;

	if (!buffer || buffersize == 0) {
		return 1;
	}

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

		return 1;
	}

	if (get_data(serial0, buffer, RECEIVE_BUFFER_SIZE) == 1) {
		return 1;
	}

	data1_len = strlen(buffer);
	buffer[data1_len] = ';';

	get_data(serial1, buffer + data1_len + 1, RECEIVE_BUFFER_SIZE - data1_len - 1);

	return 0;
}

static int get_and_print_data(int publish, int print, int printRaw) {
	char value_buffer[RECEIVE_BUFFER_SIZE];
	char *currentValuePtr;
	size_t index = 0;
	float value;

	if (read_values_from_sensors(value_buffer, RECEIVE_BUFFER_SIZE) == 1) {
		return 1;
	}

	if (publish && !daemonizeFlag) {
		if (open_mqtt_connection(&client, broker_address, CLIENT_ID) == 1) {
			return 1;
		}
	}

	currentValuePtr = strtok(value_buffer, ";");
	while (currentValuePtr && index < sensors_size) {

		sscanf(currentValuePtr, "%f", &value);

		value *= sensors[index].factor;

		if (print) {
			const char *unit = (sensors[index].unit) ? sensors[index].unit : "";
			printf("%s: %4.*f %s\n", sensors[index].value_name, sensors[index].value_is_float, value, unit);
		}

		if (publish) {
			if (mqtt_send_value(&client, sensors[index].topic, value) == 1) {
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
