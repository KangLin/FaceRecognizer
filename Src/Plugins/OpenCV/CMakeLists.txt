option(USE_OPENCV "Use opencv" ON)
if(NOT USE_OPENCV)
    return()
endif()

PROJECT(PluginConverFormatOpenCV)

find_package(OpenCV)
message(STATUS "Plug OpenCV:${OpenCV_FOUND};${OpenCV_LIBS}")
if(NOT OpenCV_FOUND)
    return()
endif()

set(SOURCES_FILES
    ConverFormatOpenCV.cpp
    )
set(HEADER_FILES
    ConverFormatOpenCV.h
    )

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/ConverFormat)
endif()
ADD_PLUGIN_TARGET(NAME ${PROJECT_NAME}
    SOURCE_FILES ${SOURCES_FILES} ${HEADER_FILES}
    PRIVATE_LIBS ${OpenCV_LIBS} FaceRecognizer
    PRIVATE_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS}
    PRIVATE_DEFINITIONS OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR}
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/ConverFormat
    INSTALL_DIR ${INSTALL_PATH}
    VERSION ${FaceRecognizer_VERSION})

foreach(opencv_component ${OpenCV_LIBS})
    INSTALL_TARGETS(TARGETS ${opencv_component})
    
    if((opencv_component STREQUAL "opencv_dnn")
            AND (OpenCV_VERSION VERSION_GREATER_EQUAL "3.4.0"))
        include(OpenCV_DNN.cmake)
    endif()
    
    if(opencv_component STREQUAL "opencv_face"
            AND OpenCV_VERSION VERSION_GREATER_EQUAL "3.4.0")
        include(OpenCV_Face.cmake)
    endif()
endforeach()
