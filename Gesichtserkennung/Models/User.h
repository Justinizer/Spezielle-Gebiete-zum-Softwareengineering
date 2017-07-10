/**
 * @file User.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 26.06.2017
 * @brief Header file of User class.
 *
 */

#ifndef GESICHTSERKENNUNG_USER_H
#define GESICHTSERKENNUNG_USER_H

#include <string>
#include <opencv2/core.hpp>
#include "../Handler/ImageHandler.h"


/**
 * @brief User class to define users.
 *
 * The User class keeps ID, name, images and last confidence number of the users.
 */
class User {
public:
    User(int ID);
    User(int ID, std::string name);
    User(int ID, std::string name, std::vector<cv::Mat*> images);
    ~User();
    void setName(std::string);
    std::string getName();
    void addImage(cv::Mat*);
    void addImage(std::istream*);
    std::vector<cv::Mat*> getImages();
    int getID();
    void setConfidence(double newConfidence);
    double getConfidence();


private:
    std::string name;
    int ID;
    std::vector<cv::Mat*> images;
    double confidence;
};


#endif //GESICHTSERKENNUNG_USER_H
