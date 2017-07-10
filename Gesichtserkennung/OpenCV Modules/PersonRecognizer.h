/**
 * @file PersonRecognizer.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Header file of PersonRecognizer class.
 *
 */

#ifndef GESICHTSERKENNUNG_PERSONRECOGNIZER_H
#define GESICHTSERKENNUNG_PERSONRECOGNIZER_H
#include "opencv2/core.hpp"
#include <opencv2/face/facerec.hpp>
#include "../Handler/FileHandler.h"
#include "../Handler/ImageHandler.h"

/**
 * @brief PersonRecognizer class to recognize persons from a given image.
 *
 * The PersonRecognizer class recognizes person or persons from given set of images by using LBPH by openCV.
 */
class PersonRecognizer {
public:
    static PersonRecognizer* getInstance();
    User* recognizePersonFromFace(cv::Mat faceFrame);
    std::vector<User*> recognizePersonsFromFaces(std::vector<cv::Mat*> faceFrames);

protected:
    PersonRecognizer();

private:
    static PersonRecognizer *instance;
    cv::Ptr<cv::face::FaceRecognizer> faceRecognizer;
    bool pRecognizable = false;
    std::vector<User*> users;
};


#endif //GESICHTSERKENNUNG_PERSONRECOGNIZER_H
