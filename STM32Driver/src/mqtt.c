#include "mqtt.h"
#include "commands.h"
#include <string.h>
#include <syslog.h>
#include <stdlib.h>

extern const char *BRIGHTNESS_TOPIC;
extern const char *serial0;


int open_mqtt_connection(MQTTClient *client, const char *broker_address, const char *client_id) {
	MQTTClient_connectOptions opt = MQTTClient_connectOptions_initializer;
	int result;

	if (!client || !broker_address) {
		return 1;
	}


	result = MQTTClient_create(client, broker_address, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (result != MQTTCLIENT_SUCCESS) {
		printf("Error creating client! Result: %d\n", result);
		return 1;
	}

	opt.keepAliveInterval = 20;
	opt.cleansession = 1;


	MQTTClient_setCallbacks(*client, NULL, mqtt_lost_connection, mqtt_message_arrived, mqtt_message_delivered);


	result = MQTTClient_connect(*client, &opt);
	if (result != MQTTCLIENT_SUCCESS) {
		printf("Error connecting to broker! Result: %d\n", result);
		return 1;
	}


	return 0;
}

int close_mqtt_connection(MQTTClient *client) {
	if (!client) {
		return 1;
	}

	if (MQTTClient_isConnected(*client)) {
		MQTTClient_disconnect(*client, 10000);
	}
	MQTTClient_destroy(client);

	return 0;
}


int mqtt_send_value(MQTTClient *client, const char *topic, float value) {
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


void mqtt_lost_connection(void *context, char *cause) {

}


int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
	if (topicLen == 0) {
		topicLen = strlen(topicName);
	}

	if (strncmp(BRIGHTNESS_TOPIC, topicName, topicLen) == 0) {
		char *msgBuffer = malloc(message->payloadlen + 1);
		if (msgBuffer) {

			memcpy(msgBuffer, message->payload, message->payloadlen);
			msgBuffer[message->payloadlen] = '\0';

			syslog(LOG_INFO, "Received message: %s", msgBuffer);

			int brightness;
			int elements;

			elements = sscanf(msgBuffer, "%d", &brightness);
			if (elements == 1) {

				syslog(LOG_INFO, "Setting brightness to %d", brightness);
				set_brightness(serial0, brightness);
			}

			free(msgBuffer);
		}
	}

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void mqtt_message_delivered(void *context, MQTTClient_deliveryToken dt) {

}