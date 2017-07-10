/**
 * @file RPiController.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 26.06.2017
 * @brief Header file of RPiController class.
 *
 */

#include "../DataLoader/JSONSettings.h"
extern "C" {
#include "wiringPi.h"
};

#ifndef GESICHTSERKENNUNG_RPICONTROLLER_H
#define GESICHTSERKENNUNG_RPICONTROLLER_H

/**
 * @brief RPiController class to control the external components of the raspberry pi.
 *
 * The RPiController class controls the LEDs and reads the state of the button which are connected to the raspberry pi.
 */
class RPiController {
public:
    static RPiController* getInstance();
    void setRedLED(bool state);
    void setGreenLED(bool state);
    void setYellowLED(bool state);
    bool getButton1();
private:
    RPiController();
    static RPiController* rpi;
    JSONSettings *js;
    bool yellowState, redState, greenState;
};


#endif //GESICHTSERKENNUNG_RPICONTROLLER_H
