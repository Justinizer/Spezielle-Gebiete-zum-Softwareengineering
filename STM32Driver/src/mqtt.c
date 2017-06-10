#include "mqtt.h"
#include <string.h>

int open_mqtt_connection(MQTTClient *client, const char *broker_address, const char *client_id) {
	MQTTClient_connectOptions opt = MQTTClient_connectOptions_initializer;
	int result;

	if (!client || !broker_address) {
		return 1;
	}


	MQTTClient_create(client, broker_address, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	opt.keepAliveInterval = 20;
	opt.cleansession = 1;

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

	MQTTClient_disconnect(*client, 10000);
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