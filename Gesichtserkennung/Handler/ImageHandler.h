/**
 * @file ImageHandler.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Header file of ImageHandler class.
 *
 */

#ifndef GESICHTSERKENNUNG_IMAGEHANDLER_H
#define GESICHTSERKENNUNG_IMAGEHANDLER_H

#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "../DataLoader/JSONSettings.h"

/**
 * @brief ImageHandler to manipulate images.
 *
 * The ImageHandler class manipulates OpenCV images and contains static methods to do this.
 */
class ImageHandler {
public:
    static void toGray(cv::Mat * frame);
    static void drawRectangleAroundFace(cv::Rect_<int> face, cv::Mat *frame);
    static void drawRectangleAroundFaces(std::vector<cv::Rect_<int>> faces, cv::Mat *frames);
    static cv::Mat * cropFrameToFaceFrame(cv::Rect_<int> face, cv::Mat frame);
    static std::vector<cv::Mat *> cropFrameToFaceFrames(std::vector<cv::Rect_<int>> faces, cv::Mat frame);
    static void resizeImage(cv::Mat * frame);
    static void resizeImages(std::vector<cv::Mat*> frames);
    static void resizeImage(cv::Mat * frame, int sizeX, int sizeY);
    static void resizeImages(std::vector<cv::Mat*> frames, int sizeX, int sizeY);
};


#endif //GESICHTSERKENNUNG_IMAGEHANDLER_H
