/**
 * @file FaceDetection.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 12.06.2017
 * @brief Header file of FaceDetection class.
 *
 */

#ifndef GESICHTSERKENNUNG_FACEDETECTION_H
#define GESICHTSERKENNUNG_FACEDETECTION_H

#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>


/**
 * @brief FaceDetection class to detect faces from a given image.
 *
 * The FaceDetection class detects face or faces from given image by using Haar Cascade method by openCV.
 */
class FaceDetection {
public:
    FaceDetection(std::string);
    void detectFacesOfFrame(cv::Mat frame, std::vector<cv::Rect_<int>> * faces);
private:
    cv::CascadeClassifier haar_cascade;
};


#endif //GESICHTSERKENNUNG_FACEDETECTION_H
