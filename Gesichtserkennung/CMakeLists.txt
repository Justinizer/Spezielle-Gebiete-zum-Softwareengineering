# Sets the required CMake Version of the project to >=3.7.
cmake_minimum_required(VERSION 3.7)
# Projectname
project(Gesichtserkennung)
# include directorities to the project for finding other classes like rapidjson OpenCV etc.
include_directories(arm-cross/include)
# Set OpenCV Modul to required for this project. It is in the share folder.
find_package(OpenCV REQUIRED PATHS arm-cross/share/OpenCV)
# Link Directorities for use of libraries like WirePi and OpenCV.
link_directories(arm-cross/lib /usr/arm-linux-gnueabihf/lib)
# Use the ARM GCC Compiler for cross compiling.
set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
# Use the ARM G++ Compiler for cross compiling.
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)
# Set the standard to C++11.
set(CMAKE_CXX_STANDARD 11)

#excluded DataLoader/MySQLController.cpp DataLoader/MySQLController.h
# All source files used by project. Class files and main.
set(SOURCE_FILES main.cpp "OpenCV Modules/FaceDetection.cpp" "OpenCV Modules/FaceDetection.h" Handler/ImageHandler.cpp Handler/ImageHandler.h "OpenCV Modules/PersonRecognizer.cpp" "OpenCV Modules/PersonRecognizer.h" Handler/FileHandler.cpp Handler/FileHandler.h DataLoader/JSONSettings.cpp DataLoader/JSONSettings.h RPI/RPiController.cpp RPI/RPiController.h Models/User.cpp Models/User.h Network/MQTTClientCpp.cpp Network/MQTTClientCpp.h)

# Make one executable out of the source files called "Gesichtserkennung"
add_executable(Gesichtserkennung ${SOURCE_FILES})
# excluded mysqlcppconn

# Link libraries to the make process. Another way can be: make -lwiringPi -lpthread -lcrypt. (pthread is required by OpenCV, wiringPi is required by RPiController and crypt is required by WiringPi)
target_link_libraries( Gesichtserkennung ${OpenCV_LIBS} wiringPi pthread crypt paho-mqtt3c)
