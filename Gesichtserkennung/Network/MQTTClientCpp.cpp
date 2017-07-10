/**
 * @file MQTTClientCpp.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 08.07.2017
 * @brief Class file of MQTTClientCpp class.
 *
 */

#include <vector>
#include "MQTTClientCpp.h"

MQTTClientCpp* MQTTClientCpp::instance = 0;

MQTTClientCpp *MQTTClientCpp::getInstance() {
    if(instance == 0) instance = new MQTTClientCpp();
    return instance;
}

/**
 * \brief The constructor of the MQTTClientCpp class
 *
 * The constructor creates a MQTT Client and tries to start the connection.
 *
 */
MQTTClientCpp::MQTTClientCpp() {
    this->js = JSONSettings::getInstance();
    this->conn_opts = MQTTClient_connectOptions_initializer;
    this->msg = MQTTClient_message_initializer;
    MQTTClient_create(&this->client, js->getMQTTAddress().c_str(), js->getMQTTClientId().c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if((this->rc = MQTTClient_connect(this->client, &this->conn_opts)) != MQTTCLIENT_SUCCESS) {
        throw "Can't connect to MQTT Server!";
    }
}


/**
 * \brief Send a string message
 *
 * This function sends a given string message.
 *
 * \param[in] msg message to send
 */
void MQTTClientCpp::sendMsg(std::string msg) {
    std::vector<char> msgChars(msg.begin(), msg.end());
    msgChars.push_back('\0');
    this->sendCharMsg(&msgChars[0]);
}

/**
 * \brief Send a char message
 *
 * This function sends a given char message.
 *
 * \param[in] msg message to send
 */
void MQTTClientCpp::sendCharMsg(char *msg) {
    this->msg.payload = msg;
    this->msg.payloadlen = strlen(msg);
    this->msg.qos = 1;
    this->msg.retained = 0;
    MQTTClient_publishMessage(this->client, this->js->getMQTTTopic().c_str(), &this->msg, &this->token);
    this->rc = MQTTClient_waitForCompletion(this->client, this->token, 10000L);
}


/**
 * \brief The deconstructor of the MQTTClientCpp class
 *
 * The deconstructor disconnects and deletes the client.
 *
 */
MQTTClientCpp::~MQTTClientCpp() {
    MQTTClient_disconnect(this->client, 10000);
    MQTTClient_destroy(&this->client);
}

