/**
 * @file RPiController.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 26.06.2017
 * @brief Class file of RPiController class.
 *
 */

#include "RPiController.h"

RPiController* RPiController::rpi = 0;

RPiController *RPiController::getInstance() {
    if(rpi == 0) rpi = new RPiController();
    return rpi;
}

/**
 * \brief the constructor of the RPiController class
 *
 * This constructor sets the output and input pins.
 *
 */
RPiController::RPiController() {
    wiringPiSetup();
    js = JSONSettings::getInstance();
    pinMode(js->getGPIORedLED(), OUTPUT);
    pinMode(js->getGPIOGreenLED(), OUTPUT);
    pinMode(js->getGPIOYellowLED(), OUTPUT);
    pinMode(js->getGPIOButton1(), INPUT);
}

/**
 * \brief turn ON/OFF red LED
 *
 * This function adjusts the state of the red LED (on or off) according to the given state when there is change in state of the red LED.
 * The red LED is on when the person is neider the user "Justin" nor "Gamze".
 *
 * \param[in] state 1->ON, 0->OFF
 */
void RPiController::setRedLED(bool state) {
    if(state != this->redState) {
        digitalWrite(js->getGPIORedLED(), (int)state);
        this->redState = state;
    }
}


/**
 * \brief turn ON/OFF green LED
 *
 * This function adjusts the state of the green LED (on or off) according to the given state when there is change in state of the green LED.
 * The green LED is on when the person is the user "Justin".
 *
 * \param[in] state 1->ON, 0->OFF
 */
void RPiController::setGreenLED(bool state) {
    if(state != this->greenState) {
        digitalWrite(js->getGPIOGreenLED(), (int)state);
        this->greenState = state;
    }
}


/**
 * \brief turn ON/OFF yellow LED
 *
 * This function adjusts the state of the yellow LED (on or off) according to the given state when there is change in state of the yellow LED.
 * The yellow led is on when the person is the user "Gamze".
 *
 * \param[in] state 1->ON, 0->OFF
 */
void RPiController::setYellowLED(bool state) {
    if(state != this->yellowState) {
        digitalWrite(js->getGPIOYellowLED(), (int)state);
        this->yellowState = state;
    }
}

/**
 * \brief read the state of the button
 *
 * This function reads the button and returns its value.
 * When the button is pushed, all LEDs are OFF.
 *
 * \return bool
 * \retval 1 button is pushed
 * \retval 0 button is not pushed
 */
bool RPiController::getButton1() {
    return digitalRead(js->getGPIOButton1());
}

