#ifndef _MQTT_H_
#define _MQTT_H_

#include <MQTTClient.h>

int open_mqtt_connection(MQTTClient *client, const char *broker_address, const char *client_id);
int close_mqtt_connection(MQTTClient *client);
int mqtt_send_value(MQTTClient *client, const char *topic, float value);


// Callback functions:
void mqtt_lost_connection(void *context, char *cause);
int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void mqtt_message_delivered(void *context, MQTTClient_deliveryToken dt);

#endif