/**
 * @file JSONSettings.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 19.06.2017
 * @brief Class file of JSONSettings class.
 *
 */

#include "JSONSettings.h"

JSONSettings* JSONSettings::instance = 0;

/**
 * \brief Get an instance of Singleton Pattern class.
 *
 *      This static function returns an instance of JSONSettings to avoid multiple instances of this class. (Singleton Pattern)
 * \return instance of the class JSONSettings.
 */
JSONSettings* JSONSettings::getInstance() {
    if(instance == 0) instance = new JSONSettings();
    return instance;
}

/**
 * \brief Constructor to open the "settings.json".
 *
 *      This constructor opens the "settings.json" file to read the predefined settings.
 *
 */
JSONSettings::JSONSettings() {
    FILE *fp = fopen("settings.json", "r");
    char readBuffer[65536];
    rapidjson::FileReadStream s(fp, readBuffer, sizeof(readBuffer));
    this->d.ParseStream(s);
}

/**
 * \brief Get the ID of the camera.
 *
 *      This function returns the ID of the camera.
 *
 * \return int The ID of the integrated Camera
 */
int JSONSettings::getVideoID() {
    const rapidjson::Value& videoIDValue = this->d["VideoID"];
    return videoIDValue.GetInt();
}

/**
 * \brief Get the key to stop program.
 *
 *      This function listens to a specific key.
 *      When the specific key is pushed, the software will terminate.
 *
 * \return int Key to stop the program
 */
int JSONSettings::getAbortKey() {
    const rapidjson::Value& abortKeyValue = this->d["AbortKey"];
    return abortKeyValue.GetInt();
}

/**
 * \brief Shoot pictures of persons.
 *
 *      This function listens to a specific key.
 *      When the specific key is pushed, pictures from current detected persons of the camera are taken.
 *
 * \return int key to take a picture
 */
int JSONSettings::getPictureKey() {
    const rapidjson::Value& pictureKeyValue = this->d["PictureKey"];
    return pictureKeyValue.GetInt();
}

/**
 * \brief Get the path to haarcascade.
 *
 * This function returns the path of the haar cascade file.
 *
 * \return string path to HaarCascade file
 */
std::string JSONSettings::getPathHaarCascade() {
    const rapidjson::Value& pathHaarCascadeValue = this->d["PathHaarCascade"];
    return pathHaarCascadeValue.GetString();
}

/**
 * \brief Get the path to picture folder.
 *
 *      This function returns the path of the pictures of the users.
 *
 * \return string path to the folder of the pictures
 */
std::string JSONSettings::getPathPictureFolder() {
    const rapidjson::Value& pathPictureFolderValue = this->d["PathPictureFolder"];
    return pathPictureFolderValue.GetString();
}

/**
 * \brief Get the pin of red LED.
 *
 * This function returns the pin number of the red LED.
 *
 * \return int pin value of the red LED
 */
int JSONSettings::getGPIORedLED() {
    const rapidjson::Value& gpioRedLEDValue = this->d["GPIORedLED"];
    return gpioRedLEDValue.GetInt();
}


/**
 * \brief Get the pin of green LED.
 *
 * This function returns the pin number of the green LED.
 *
 * \return int value to gpio pin for green LED.
 */
int JSONSettings::getGPIOGreenLED() {
    const rapidjson::Value& gpioGreenLEDValue = this->d["GPIOGreenLED"];
    return gpioGreenLEDValue.GetInt();
}


/**
 * \brief Get the pin of yellow LED.
 *
 * This function returns the pin number of the yellow LED.
 *
 * \return int value to gpio pin for yellow LED.
 */
int JSONSettings::getGPIOYellowLED() {
    const rapidjson::Value& gpioYellowLEDValue = this->d["GPIOYellowLED"];
    return gpioYellowLEDValue.GetInt();
}


/**
 * \brief Get the pin of button 1.
 *
 * This function returns the pin number of the button.
 *
 * \return int value to gpio pin for button 1.
 */
int JSONSettings::getGPIOButton1() {
    const rapidjson::Value& gpioButton1Value = this->d["GPIOButton1"];
    return gpioButton1Value.GetInt();
}

/**
 * \brief Get default img width.
 *
 * This function returns the width to resize the images.
 *
 * \return int value to get the default width of a image.
 */
int JSONSettings::getDefaultImgWidth() {
    const rapidjson::Value& defaultImgWidthValue = this->d["DefaultImgWidth"];
    return defaultImgWidthValue.GetInt();
}


/**
 * \brief Get default img height.
 *
 * This function returns the height to resize the images.
 *
 * \return int value to get the default height of a image.
 */
int JSONSettings::getDefaultImgHeight() {
    const rapidjson::Value& defaultImgHeightValue = this->d["DefaultImgHeight"];
    return defaultImgHeightValue.GetInt();
}

/**
 * \brief Get Hostname of db.
 *
 * This function returns the host name of the database.
 *
 * \return string value to get the host name of the databse.
 */
std::string JSONSettings::getHost() {
    const rapidjson::Value& hostValue = this->d["Host"];
    return hostValue.GetString();
}


/**
 * \brief Get username of db.
 *
 * This function returns the user name of the database.
 *
 * \return string value to get the user name of the database.
 */
std::string JSONSettings::getDatabaseUser() {
    const rapidjson::Value& userValue = this->d["DatabaseUser"];
    return userValue.GetString();
}


/**
 * \brief Get user password of db.
 *
 * This function returns the user password for the database.
 *
 * \return string value to get the user password of the database.
 */
std::string JSONSettings::getPassword() {
    const rapidjson::Value& passwordValue = this->d["DatabasePassword"];
    return passwordValue.GetString();
}

/**
 * \brief Get maxmimum recognized confidence number.
 *
 * This function returns the confidence number that is used to show that one of the user is recognized.
 *
 * \return double value to get the maximum confidence to recognize a person.
 */
double JSONSettings::getRecognizedConfidence() {
    const rapidjson::Value& confidenceRecognizedValue = this->d["RecognizedConfidence"];
    return confidenceRecognizedValue.GetDouble();
}


/**
 * \brief Get minimum unrecognized confidence number.
 *
 * This function returns the confidence number that is used to show that the person from the picture is definitely not recognized.
 *
 * \return double value to get the minimum confidence to classify a person as unrecognized.
 */
double JSONSettings::getUnrecognizedConfidence() {
    const rapidjson::Value& confidenUnrecognizedeValue = this->d["UnrecognizedConfidence"];
    return confidenUnrecognizedeValue.GetDouble();
}

/**
 * \brief Get the address for MQTT.
 *
 * This function returns the address for the MQTT Network protcol.
 *
 * \return string value to get the address for MQTT.
 */
std::string JSONSettings::getMQTTAddress() {
    const rapidjson::Value& mqttAddressValue = this->d["MQTTAddress"];
    return mqttAddressValue.GetString();
}

/**
 * \brief Get the Client ID for MQTT.
 *
 * This function returns the address for the MQTT Network protcol.
 *
 * @return string value to get the Client ID for MQTT.
 */
std::string JSONSettings::getMQTTClientId() {
    const rapidjson::Value& MQTTClientIdValue = this->d["MQTTClientID"];
    return MQTTClientIdValue.GetString();
}

/**
 * \brief Get the defined topic for MQTT.
 *
 * This function returns the topic for a MQTT message.
 *
 * @return string value to get the topic for MQTT.
 */
std::string JSONSettings::getMQTTTopic() {
    const rapidjson::Value& MQTTTopicValue = this->d["MQTTTopic"];
    return MQTTTopicValue.GetString();
}

/**
 * \brief Get the defined message for MQTT.
 *
 * This function returns the message for a MQTT message.
 *
 * @return string value to get the message for MQTT.
 */
std::string JSONSettings::getMQTTMessage() {
    const rapidjson::Value& MQTTMessageValue = this->d["MQTTMessage"];
    return MQTTMessageValue.GetString();
}



