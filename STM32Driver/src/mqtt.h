#ifndef _MQTT_H_
#define _MQTT_H_

#include <MQTTClient.h>

/**
 * Opens a new connection to the MQTT broker.
 * @param client Pointer to client structure.
 * @param broker_address Address of the broker.
 * @param client_id ID of this client.
 * @return 0 if a new connection to the broker could be established. 1 otherwise.
 */
int open_mqtt_connection(MQTTClient *client, const char *broker_address, const char *client_id);

/**
 * Cloese an open MQTT connection.
 * @param client Client which connection should be closed.
 * @return 0 if the connection was closed successfully. 1 otherwise.
 */
int close_mqtt_connection(MQTTClient *client);

/**
 * Sends a floating point value to the broker.
 * @param client Client connection which should be used for the transfer.
 * @param topic Topic name of the topic to which the valu should be published.
 * @param value The value which should be published.
 * @return 0 if the value was published. 1 otherwise.
 */
int mqtt_send_value(MQTTClient *client, const char *topic, float value);


// Callback functions:

/**
 * Callback function that is called when a connection was lost.
 */
void mqtt_lost_connection(void *context, char *cause);

/**
 * Callback function that is called when a message was received.
 * @param context
 * @param topicName The name of the topic to which the nessage was published.
 * @param topicLen Length of the topicName parameter.
 * @param message The message that was received.
 * @return Always 1.
 */
int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);

/**
 * Callback function that is called whe a message was delivered.
 * @param context
 * @param dt Delivery token of the message.
 */
void mqtt_message_delivered(void *context, MQTTClient_deliveryToken dt);

#endif