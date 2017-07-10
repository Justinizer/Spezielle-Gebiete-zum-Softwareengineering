/**
 * @file ImageHandler.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Class file of ImageHandler class.
 *
 */

#include "ImageHandler.h"

/**
 * @brief RGB Image to Grayscale.
 *
 * This function turns the given RGB image to gray scaled image.
 * @param[in,out] frame Give a picture you want turn to grayscale.
 */
void ImageHandler::toGray(cv::Mat * frame) {
    cv::cvtColor(*frame, *frame, CV_RGB2GRAY);
}

/**
 * @brief Draw Rectangle around a face.
 *
 * This function draws a green (CV_RGB(0, 255,0)) rectangle around the face (the coordinates of the face) from the the given frame.
 * @param[in] face Enter positions of a face in a image.
 * @param[in,out] frame Give a picture which contains the face and you want to draw a rectangle around the face.
 */
void ImageHandler::drawRectangleAroundFace(cv::Rect_<int> face, cv::Mat *frame) {
    cv::rectangle(*frame, face, CV_RGB(0, 255,0), 1);
}

/**
 * @brief Draw Rectangle around faces.
 *
 * This function takes faces (the coordinates of the faces of the frames) and the frame.
 * Then, it calls drawRectangleAroundFace() function and draws rectangles around the faces.
 * @param[in] faces Enter a set of positions of faces in a image
 * @param[in,out] frame Give a picture which contain faces and you want draw rectangles around the faces.
 */
void ImageHandler::drawRectangleAroundFaces(std::vector<cv::Rect_<int>> faces, cv::Mat *frame) {
    for(cv::Rect_<int> face : faces) {
        ImageHandler::drawRectangleAroundFace(face, frame);
    }
}

/**
 * @brief Crop picture to a picture with face only.
 *
 * This function takes the face position and the frame (picture from the camera).
 * Then, it cuts the face from the frame and returns the frame as the face.
 *
 * @param[in] face Give positions of the face in the picture
 * @param[in] frame Give the picture which contains the face.
 * @return A image (Mat object) which contains the face only.
 */
cv::Mat * ImageHandler::cropFrameToFaceFrame(cv::Rect_<int> face, cv::Mat frame) {
    return new cv::Mat(frame, face);
}

/**
 * @brief Crop picture to a set of pictures with faces only.
 *
 * This function takes the positions of the faces and the frame the faces are in.
 * Then it calls the cropFrameToFaceFrame() function for every face (position) and creates a vector with faces(cv::Mat).
 * It returns the fames vector (detected faces).
 *
 * @param[in] faces Give positions of the faces in the picture.
 * @param[in] frame Give the picture which contains the faces.
 * @return A set of images with the faces only.
 */
std::vector<cv::Mat *> ImageHandler::cropFrameToFaceFrames(std::vector<cv::Rect_<int>> faces, cv::Mat frame) {
    std::vector<cv::Mat *> frames;
    cv::Mat * faceFrame;
    for(cv::Rect_<int> face : faces)  {
        faceFrame = ImageHandler::cropFrameToFaceFrame(face, frame);
        frames.push_back(faceFrame);
    }
    return frames;
}

/**
 * @brief Resize image to a given size.
 *
 * This function resizes the given frame (cv::Mat image) according to given width (sizeX) and height (sizeY) information.
 *  @param[in,out] frame The image which should be resized.
 *  @param[in] sizeX Give a new image width.
 *  @param[in] sizeY Give a new image height.
 */
void ImageHandler::resizeImage(cv::Mat * frame, int sizeX, int sizeY) {
    cv::resize(*frame, *frame, cv::Size_<int>(sizeX, sizeY), 1.0, 1.0, cv::INTER_CUBIC);
}

/**
 * @brief Resize images to a given size.
 *
 * This function takes a vector with pictures (cv::Mat image).
 * Then, it resizes them according to given width (sizeX) and height (sizeY) information by calling resizeImage() function.
 * @param[in,out] frames The set of images which should be resized.
 * @param[in] sizeX Give a new image width.
 * @param[in] sizeY Give a new image height.
 */
void ImageHandler::resizeImages(std::vector<cv::Mat*> frames, int sizeX, int sizeY) {
    for(cv::Mat * frame : frames){
        resizeImage(frame, sizeX, sizeY);
    }
}

/**
 * @brief Resize image to predefined size.
 *
 * This function resizes a given frame (cv::Mat image) according to the predefined width and height information from the "jsettings.json" file
 * @param[in,out] frame The image which should be resized.
 */
void ImageHandler::resizeImage(cv::Mat * frame) {
    JSONSettings* js = JSONSettings::getInstance();
    cv::resize(*frame, *frame, cv::Size_<int>(js->getDefaultImgWidth(), js->getDefaultImgHeight()), 1.0, 1.0, cv::INTER_CUBIC);
}

/**
 * @brief Resize images to predefined size.
 *
 * This function resizes a given set of frames (cv::Mat images) by calling resizeImage(frame) function according to the predefined width and height information from the "jsettings.json" file
 * @param[in,out] frames The images which should be resized.
 *
 */
void ImageHandler::resizeImages(std::vector<cv::Mat*> frames) {
    for(cv::Mat * frame : frames) {
        resizeImage(frame);
    }
}
