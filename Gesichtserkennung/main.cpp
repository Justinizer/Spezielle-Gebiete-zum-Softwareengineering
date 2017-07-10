/**
 * @file main.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Main file.
 *
 * This file contains the main method (start point) of the program.
 *
 */

#include <iostream>
#include <unistd.h>
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "Handler/ImageHandler.h"
#include "OpenCV Modules/FaceDetection.h"
#include "OpenCV Modules/PersonRecognizer.h"
#include "RPI/RPiController.h"
#include "Network/MQTTClientCpp.h"
//#include "DataLoader/MySQLController.h"

using namespace std;

/**
 * \brief A function to turn on/off LEDs.
 *
 * In this function the LED state (ON or OFF) is changed according to the new confidence number.
 * If the button is pushed, all LEDs are OFF and the recognizing part begins from the top.
 * This function also sends an MQTT message to tell the identity of the visitor.
 *
 * \param[in] users Enter a vector of users.
 * \param[in] jsSettings Enter an instance of jsettings.
 * \param[in] rpi Enter an instance of the raspberry controller.
 * \param[in] mqttClient Enter an instance of MQTT Client
 *
 */
void adjustLEDState(vector<User*> users, JSONSettings* jsSettings, RPiController* rpi, MQTTClientCpp* mqttClient){
    for(User* user : users){
        cout << user->getName() << endl;
        if (user->getConfidence() <= jsSettings->getRecognizedConfidence()){
            if(user->getName() == "Gamze"){
                rpi->setYellowLED(1);
                rpi->setRedLED(0);
            }else if(user->getName() == "Justin"){
                rpi->setGreenLED(1);
                rpi->setRedLED(0);
            }
            mqttClient->sendMsg(user->getName() + jsSettings->getMQTTMessage());
        }else if (user->getConfidence() > jsSettings->getUnrecognizedConfidence()){
            rpi->setRedLED(1);
            rpi->setYellowLED(0);
            rpi->setGreenLED(0);
            mqttClient->sendMsg("Unknown" + jsSettings->getMQTTMessage());
        }
    }

    if(rpi->getButton1() == 1){
        rpi->setYellowLED(0);
        rpi->setGreenLED(0);
        rpi->setRedLED(0);
    }
}


/**
 * \brief Main function (start point)
 *
 *      In "main" function, first of all the camera is opened. Then a PersonRecognizer object created, so the recognizer is trained.
 *      After that in a endless for-loop, pictures are taken and faces are detected from these pictures and then person is asigned to a certain user with a confidence number.
 *      The confidence number shows that how similar the person from the picture to the registered user.
 *      According to confidence number, it is defined which LEDs is ON or OFF.
 *
 */
int main() {
    cout << "Starte Gesichtserkennung..." << endl;
    char key;
    cv::Mat frame, gray;
    vector<cv::Mat *> frames;
    vector<cv::Rect_<int>> faces;
    vector<User *> users;

    try {
        MQTTClientCpp* mqttClient = MQTTClientCpp::getInstance();
        JSONSettings* jsSettings = JSONSettings::getInstance();
        RPiController* rpi = RPiController::getInstance();

        cv::VideoCapture cap(jsSettings->getVideoID());
        if (!cap.isOpened()) throw "Video device can't open!";
        FaceDetection fd(jsSettings->getPathHaarCascade());

        PersonRecognizer *pr = PersonRecognizer::getInstance();

        //MySQLController myController;
        //myController.getUsersFromDatabase(jsSettings->getHost(), jsSettings->getDatabaseUser(), jsSettings->getPassword());

        for (;;) {
            //An RGB frame is taken from the camera and turned in to gray scale.
            cap >> frame;
            if(frame.empty()) throw "No captured frame!";
            gray = frame.clone();
            ImageHandler::toGray(&gray);
            //Face or faces detected from the gray scaled frame.
            fd.detectFacesOfFrame(gray, &faces);
            //If at least one face detected, it is cut from the frame, resized and send to be recognized.
            //Then, users confidence number are updated.
            if (faces.size() > 0) {
                frames = ImageHandler::cropFrameToFaceFrames(faces, gray);
                ImageHandler::resizeImages(frames);
                if (frames.size() > 0) {
                    users = pr->recognizePersonsFromFaces(frames);
                }
            }

            //Next 2 line are used for the konsole application to show to detected face in a green rectangle.
            //ImageHandler::drawRectangleAroundFaces(faces, &frame);
            //cv::imshow("video", frame);

            //If "b" key is pushed, a picture is taken and saved into a file.
            key = (char) cv::waitKey(20);
            if (key == jsSettings->getPictureKey()) {
                FileHandler::savePictures(frames, "");
            }

            //Next function is called to adjust the LED condition according to new confidence number and to send a MQTT message to show which user at the door.
            adjustLEDState(users, jsSettings, rpi, mqttClient);

            //If "escape" key is pushed, the face recognition cycle is terminated.
            if (key == jsSettings->getAbortKey()) break;
            usleep(0.1);
        }
    } catch(exception& e) {
        cerr << "Exception catched: " << e.what() << endl;
    } catch(char const* s) {
        cerr << "Exception catched: " << s << endl;
    }

    //After the face recognition is terminated, cv::Mat images and User* users deleted to free the memory space.
    for (cv::Mat *frame : frames) delete frame;
    for(User* user: users) delete user;

    return 0;
}
