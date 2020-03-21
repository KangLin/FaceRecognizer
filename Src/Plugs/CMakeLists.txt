cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(FaceRecognizerPlugs)

add_subdirectory(Seeta)
add_subdirectory(libfacedetection)
add_subdirectory(OpenCV)
