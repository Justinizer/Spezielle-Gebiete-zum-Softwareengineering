/**
 * @file User.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 26.06.2017
 * @brief Class file of User class.
 *
 */


#include <opencv2/imgcodecs/imgcodecs_c.h>
#include "User.h"

/**
 * \brief A Constructor of the User Class
 *
 * This constructor creates a user with an ID.
 *
 * \param[in] ID of the user
 */
User::User(int ID) {
    this->ID = ID;
}

/**
 * \brief A Constructor of the User Class
 *
 * This constructor creates a user with an ID and name.
 *
 * \param[in] ID of the user
 * \param[in]name of the user
 */
User::User(int ID, std::string name) {
    this->ID = ID;
    this->name = name;
}


/**
 * \brief A Constructor of the User Class
 *
 * This constructor creates a user with an ID, name and a set of images.
 *
 * \param[in] ID of the user
 * \param[in] name of the user
 * \param[in] images of the user
 */
User::User(int ID, std::string name, std::vector<cv::Mat*> images) {
    this->ID = ID;
    this->name = name;
    this->images = images;
}

/**
 * \brief set name of the user
 *
 * This function sets the name of the user.
 *
 * \param[in] name of the user
 */
void User::setName(std::string name) {
    this->name = name;
}

/**
 * \brief add image of the user
 *
 * This function adds image to a given user.
 *
 * \param[in] image of the user
 */
void User::addImage(cv::Mat* image) {
    ImageHandler::resizeImage(image);
    this->images.push_back(image);
}

/*void User::addImage(std::istream* imageStream){
    std::vector<char> imageByte((std::istreambuf_iterator<char>(*imageStream)), std::istreambuf_iterator<char>());
    cv::imdecode(imageByte, CV_LOAD_IMAGE_GRAYSCALE);
}*/

/**
 * \brief get name of the user
 *
 * This function returns the name of the user.
 *
 * \param[out] name of the user
 */
std::string User::getName() {
    return this->name;
}

/**
 * \brief get the images of the user
 *
 * This function returns the image set of the user.
 *
 * \param[out] images of the user
 */
std::vector<cv::Mat*> User::getImages() {
    return this->images;
}

/**
 * \brief get the ID of the user
 *
 * This function returns the ID of a user.
 *
 * \param[out] ID of the user
 */
int User::getID() {
    return this->ID;
}

/**
 * \brief set the confidence number of the user
 *
 * This function set the confidence number of a user.
 * Confidence number shows us how similar the person to the registered users.
 *
 * \param[in] newConfidence of the user
 */
void User::setConfidence(double newConfidence)
{
    this->confidence = newConfidence;
}

/**
 * \brief get the confidence number of the user
 *
 * This function returns the last confidence number of the user.
 *
 * \param[out] confidence number of the user
 */
double User::getConfidence()
{
    return confidence;
}

/**
 * \brief deconstructer of the user class
 *
 * Deconstructer deletes images so that memory is released.
 *
 */
User::~User() {
    for(cv::Mat* img : this->images) delete img;
}
