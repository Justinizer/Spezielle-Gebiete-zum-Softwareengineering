/**
 * @file PersonRecognizer.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 16.06.2017
 * @brief Class file of PersonRecognizer class.
 *
 */

#include "PersonRecognizer.h"

PersonRecognizer* PersonRecognizer::instance = 0;

PersonRecognizer* PersonRecognizer::getInstance() {
    if(instance == 0) instance = new PersonRecognizer();
    return instance;
}

/**
 * \brief recognize the person
 *
 * This function takes the detected face and updates the confidence number of the user.
 *
 * Confidence number shows us that how similar the person from the faceFrame to our user (K-nearest neighbor).
 * When confidence <= "RecognizedConfidence", the person is recognized.
 * When confiden > "UnrecognizedConfidence", the person is not one of the users that we know
 * "RecognizedConfidence" and "UnrecognizedConfidence" numbers are defined in jsettings.json
 *
 * \param[in] faceFrame the image of the person that we want to recognize
 * \return user the recognized
 * \retval nullptr person is unrecognazible or not recognized
 */
User* PersonRecognizer::recognizePersonFromFace(cv::Mat faceFrame) {
    if(!this->pRecognizable) return nullptr;
    double confidence = 0.0;
    int label = -1;
    if(!faceFrame.isContinuous()) {
        faceFrame = faceFrame.clone();
    }

    this->faceRecognizer->predict(faceFrame, label, confidence);
    for(User* user: this->users) {
        if(user->getID() == label) {
            user->setConfidence(confidence);
            return user;
        }
    }
    return nullptr;
}

/**
 * \brief recognize persons from faces
 *
 * This function takes a cv::Mat vector with face frames.
 * It sends them one by one to recognizePersonFromFace() and updates users.
 * It returns a vector with recognized users.
 *
 * \param[in] faceFrames the images of the person that we want to recognize
 * \return users that are recognized
 * \retval {} the persons are unrecognizable
 */
std::vector<User*> PersonRecognizer::recognizePersonsFromFaces(std::vector<cv::Mat*> faceFrames) {
    if(!this->pRecognizable) return {};
    std::vector<User*> users;
    User* user;
    for(cv::Mat *faceFrame : faceFrames) {
        user = this->recognizePersonFromFace(*faceFrame);
        if(user != nullptr) users.push_back(user);
    }
    return users;
}

/**
 * \brief the constructor of the PersonRecognizer class
 *
 * This constructor reads first of all the pictures from files and creates users according to file names.
 * It crreates a face recognizer, in this case with LBPH pattern.
 * Then, it trains the face recognizer with the pictures from the files.
 * Then, the software is ready to recognize.
 *
 */
PersonRecognizer::PersonRecognizer() {
    std::vector<cv::Mat> faces;
    std::vector<int> labels;
    FileHandler::readPictureFolder(&this->users);

    this->faceRecognizer = cv::face::createLBPHFaceRecognizer();

    for(User* user: this->users) {
        for(cv::Mat* img : user->getImages()) {
            faces.push_back(*img);
            labels.push_back(user->getID());
        }
    }

    if(faces.size() > 1) {
        this->faceRecognizer->train(faces, labels);
        this->pRecognizable = true;
    } else {
        std::cerr << "You need more than 1 training data to recognize persons!" << std::endl;
    }
}
