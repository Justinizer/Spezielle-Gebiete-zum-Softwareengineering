#include <string.h>		// For strerror()
#include <stdlib.h>
#include <getopt.h>		// For getopt_long()
#include <unistd.h>		// For sleep()
#include <stdio.h>
#include <errno.h>
#include "serial.h"
#include "daemonize.h"

#define COMMAND_PACKET_HEADER	0xAA
#define COMMAND_PACKET_TAIL		0xFF
#define COMMAND_GET_DATA		0x01
#define COMMAND_SET_BRIGHTNESS	0x02


int daemonizeFlag = 0;
int getDataFlag = 0;
int printDataRawFlag = 0;
int setBrightnessFlag = 0;
int setBrightness = 0;

static const struct option program_options[] = {
//	name				has_arg				flag				val
	{"daemonize",		no_argument, 		&daemonizeFlag, 	'd'},
	{"get-data", 		no_argument, 		&getDataFlag,		'g'},
	{"help",			no_argument, 		NULL,				'h'},
	{"raw",				no_argument, 		&printDataRawFlag,	'r'},
	{"set-brightness",	required_argument,	NULL,				's'},
	{0, 0, 0, 0}
};

static void print_help(const char *progname);
static int get_data(char *buffer, size_t buffersize);
static int get_and_print_data();
static int set_brightness(int brightness);

int main(int argc, char *argv[]) {
	int optionIndex;
	int c;

	if (argc == 1) {
		print_help(argv[0]);
		return 0;
	}

	while ((c = getopt_long(argc, argv, "dghrs:", program_options, &optionIndex)) != -1) {

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

			case 'r':
				printDataRawFlag = 1;
				break;

			case 's':
				setBrightnessFlag = 1;
				setBrightness = atoi(optarg);
				break;
		}
	}

	if (getDataFlag) {
		return get_and_print_data();
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

static int get_and_print_data() {
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

		sscanf(buffer, "%f;%f;%f;%f", &pm2_5, &pm10, &temperature, &humidity);

		pm2_5 /= 10.0f;
		pm10 /= 10.0f;
		temperature /= 10.0f;
		humidity /= 10.0f;

		printf("PM 2,5µm:    %4.1f µg/m³\n", pm2_5);
		printf("PM 10µm :    %4.1f µg/m³\n", pm10);
		printf("Temperature: %4.1f °C\n", temperature);
		printf("Humidity:    %4.1f %%rF\n", humidity);
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