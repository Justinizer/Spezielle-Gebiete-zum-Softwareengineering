/**
 * @file FileHandler.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Header file of FileHandler class.
 *
 */

#ifndef GESICHTSERKENNUNG_FILEHANDLER_H
#define GESICHTSERKENNUNG_FILEHANDLER_H

#include <iostream>
#include <opencv2/core.hpp>
#include "../DataLoader/JSONSettings.h"
#include "../Models/User.h"
#include <ctime>
#include <iomanip>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <fstream>
#include <dirent.h>
#include <opencv2/imgcodecs.hpp>

/**
 * @brief FileHandler class to read and save files.
 *
 * The FileHandler class can read and saves files. It reads pictures and can ready userdata of the folder structure.
 */
class FileHandler {
public:
    static void readPictureFolder(std::vector<User*> *users);
    static int savePicture(cv::Mat frame, std::string path, int);
    static int savePictures(std::vector<cv::Mat*> frames, std::string path);
private:

};


#endif //GESICHTSERKENNUNG_FILEHANDLER_H
