/**
 * @file JSONSettings.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 19.06.2017
 * @brief Header file of JSONSettings class.
 *
 */

#ifndef GESICHTSERKENNUNG_JSONSETTINGS_H
#define GESICHTSERKENNUNG_JSONSETTINGS_H

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

/**
 * \brief A class to get values from jSettings.json file.
 *
 * In this class you can get values for database, raspberry ports, used keys etc. of the jSettings.json file.
 */
class JSONSettings {
public:
    static JSONSettings* getInstance();
    int getVideoID();
    int getAbortKey();
    int getPictureKey();
    int getGPIORedLED();
    int getGPIOGreenLED();
    int getGPIOYellowLED();
    int getGPIOButton1();
    int getDefaultImgWidth();
    int getDefaultImgHeight();
    std::string getPathHaarCascade();
    std::string getPathPictureFolder();
    std::string getHost();
    std::string getDatabaseUser();
    std::string getPassword();
    std::string getMQTTAddress();
    std::string getMQTTClientId();
    std::string getMQTTTopic();
    std::string getMQTTMessage();
    double getRecognizedConfidence();
    double getUnrecognizedConfidence();
protected:
    JSONSettings();
private:
    static JSONSettings *instance;
    rapidjson::Document d;
};


#endif //GESICHTSERKENNUNG_JSONSETTINGS_H
