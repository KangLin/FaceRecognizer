option(USE_LIBFACEDETECTION "Use LibFacedetection" ON)
message(STATUS "Use LibFacedetection: ${USE_LIBFACEDETECTION}")
if(NOT USE_LIBFACEDETECTION)
    return()
endif()

project(PluginFaceLibFacedetection)

find_package(facedetection)
message(STATUS "Plugins libfacedetection:${facedetection_FOUND}")
if(NOT facedetection_FOUND)
    return()
endif()

set(SOURCES_FILES
    FaceLibfacedetection.cpp
    DetectorLibfacedetection.cpp)
set(HEADER_FILES
    FaceLibfacedetection.h
    DetectorLibfacedetection.h)

#INSTALL_TARGETS(TARGETS facedetection)

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
endif()
ADD_PLUGIN_TARGET(NAME ${PROJECT_NAME}
    NO_TRANSLATION
    SOURCE_FILES ${SOURCES_FILES} ${HEADER_FILES}
    PRIVATE_LIBS facedetection FaceRecognizer
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
    INSTALL_DIR ${INSTALL_PATH}
    INSTALL_RPATH
        "$ORIGIN:$ORIGIN/../../lib:$ORIGIN/../../lib/${CMAKE_LIBRARY_ARCHITECTURE}"
    VERSION ${FaceRecognizer_VERSION})
