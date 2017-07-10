/**
 * @file FaceDetection.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 12.06.2017
 * @brief Class file of FaceDetection class.
 *
 */

#include <opencv2/core.hpp>

#include "FaceDetection.h"

/**
 * \brief detect faces from a picture frame
 *
 * This function finds the face positions in the given picture.
 *
 * \param[in] frame Given image that we want to detect faces from
 * \param[in,out] faces The position of the faces that we detect
 */
void FaceDetection::detectFacesOfFrame(cv::Mat frame, std::vector<cv::Rect_<int>> * faces) {
    this->haar_cascade.detectMultiScale(frame, *faces);
}

/**
 * \brief constructor of the FaceDetection class
 *
 * This function loads the features of the human face (eyes, nose, lips) in haar cascade file.
 *
 * \param[in] pathHaarCascade Path of the HaarCascade file.
 */
FaceDetection::FaceDetection(std::string pathHaarCascade) {
    this->haar_cascade.load(pathHaarCascade);
}
