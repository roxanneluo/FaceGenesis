#
# This is a CMake makefile.  You can find the cmake utility and
# information about it at http://www.cmake.org
#


cmake_minimum_required(VERSION 2.8.4)

PROJECT(FaceOff)

find_package(OpenCV)
find_package(X11)
#include(../dlib-18.18/dlib/cmake)
include_directories(${OpenCV_INCLUDE_DIRS} ../dlib-18.18/)

# Tell CMake to compile a program.  We do this with the ADD_EXECUTABLE()
# statement which takes the name of the output executable and then a list of
# .cpp files to compile.  Here each example consists of only one .cpp file but
# in general you will make programs that const of many .cpp files.
ADD_EXECUTABLE(FaceOff main.cpp)
# Then we tell it to link with dlib.
TARGET_LINK_LIBRARIES(FaceOff dlib ${OpenCV_LIBS})
