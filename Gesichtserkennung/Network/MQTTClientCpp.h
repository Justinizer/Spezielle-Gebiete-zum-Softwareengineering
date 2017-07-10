/**
 * @file MQTTClientCpp.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 08.07.2017
 * @brief Header file of MQTTClientCpp class.
 *
 */

#ifndef GESICHTSERKENNUNG_MQTTCLIENT_H
#define GESICHTSERKENNUNG_MQTTCLIENT_H
#include "../DataLoader/JSONSettings.h"
#include <MQTTClient.h>

/**
 * @brief MQTTClientCpp class to send messages to MQTT Server
 *
 * The MQTTClientCpp class creates a MQTT Client and sends a string or a char message to the server.
 */
class MQTTClientCpp {
public:
    static MQTTClientCpp* instance;
    static MQTTClientCpp* getInstance();
    void sendMsg(std::string msg);
    void sendCharMsg(char* msg);
private:
    MQTTClientCpp();
    ~MQTTClientCpp();
    MQTTClient client;
    MQTTClient_connectOptions conn_opts;
    MQTTClient_message msg;
    MQTTClient_deliveryToken token;
    int rc;
    JSONSettings* js;
};


#endif //GESICHTSERKENNUNG_MQTTCLIENT_H
