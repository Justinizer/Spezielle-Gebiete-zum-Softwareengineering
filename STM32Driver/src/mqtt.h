#ifndef _MQTT_H_
#define _MQTT_H_

#include <MQTTClient.h>

int open_mqtt_connection(MQTTClient *client, const char *broker_address, const char *client_id);
int close_mqtt_connection(MQTTClient *client);
int mqtt_send_value(MQTTClient *client, const char *topic, float value);

#endif