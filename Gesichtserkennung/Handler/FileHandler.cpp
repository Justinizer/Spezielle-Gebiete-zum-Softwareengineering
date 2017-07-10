/**
 * @file FileHandler.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Class file of FileHandler class.
 *
 */

#include "FileHandler.h"

/**
 * \brief Read pictures from a given folder.
 *
 * read Pictures from a given Folder... Following structure: PersonName / IMG.pgm
 * Then, it updates the informations of the users. Names according to the file name which has pictures in it. And user images with the pictures from the files.
 * \param[out] users Save the pictures to a given vector.
 */
void FileHandler::readPictureFolder(std::vector<User*> *users) {
    JSONSettings *jsonSettings = JSONSettings::getInstance();
    std::string folderPath = jsonSettings->getPathPictureFolder();
    std::string path;
    DIR *dir = opendir(folderPath.c_str());
    DIR *dir1;
    std::string newString;
    cv::Mat* image;
    User* user;
    struct dirent *entry;
    struct dirent *entry2;
    int n = 1;

    while((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            std::cout << entry->d_name << std::endl;
            path = folderPath + "/" + entry->d_name;
            dir1 = opendir(path.c_str());
            user = new User(n, entry->d_name);

            while((entry2 = readdir(dir1)) != NULL) {
                if(entry2->d_type != DT_DIR) {
                    newString = path + "/";
                    newString += entry2->d_name;
                    image = new cv::Mat(cv::imread(newString, CV_LOAD_IMAGE_GRAYSCALE));
                    if(! image->data) {
                        std::cerr << "Could not find image: " << newString << std::endl;
                        break;
                    }
                    user->addImage(image);
                }
            }
            users->push_back(user);
            n++;
            closedir(dir1);
        }
    }
    closedir(dir);
}

/**
 * \brief Save picture to disk.
 *
 * This function saves the given picture (cv::Mat image) in to the given path as a pgm-image.
 * A picture name is created with the current time + "img" + the number of the pictures in this picture set,
 * because it can be necessary to save pictures in the same time. (For example: If there is more than one person)
 * @param[in] frame The picture (Mat object) which should be saved.
 * @param[in] path Enter a path to save picture.
 * @param[in] n The number of picture. Default is 0.
 */
int FileHandler::savePicture(cv::Mat frame, std::string path , int n=0) {
    std::time_t timeStamp = std::time(0);
    struct tm tm = *std::localtime(&timeStamp);
    std::ostringstream picName;
    picName << "img" << std::put_time(&tm, "%Y%m%d%H%M%S") << "_" << n << ".pgm";
    cv::imwrite(path + picName.str(), frame);
    return 1;
}

/**
 * \brief Save a set of pictures.
 *
 * This function saves the given pictures (a vector with cv::Mat images) into the given path by callin the savePicture() function.
 * \param[in] frames The pictures which should be saved.
 * \param[in] path Enter a path to save the pictures.
 */
int FileHandler::savePictures(std::vector<cv::Mat*> frames, std::string path) {
    int n=1;
    for(cv::Mat * frame : frames) {
        savePicture(*frame, path, n);
        n++;
    }
    return 1;
}
