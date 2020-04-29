#PROJECT(PlugFaceOpencvDNN)

set(SOURCES_FILES
    DetectorOpenCVDNN.cpp
 #   FaceOpenCVDNN.cpp
    )
set(HEADER_FILES
    DetectorOpenCVDNN.h
  #  FaceOpenCVDNN.h
    )

## Install target
#set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
#if(NOT ANDROID)
#    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
#endif()
#ADD_PLUG_TARGET(NAME ${PROJECT_NAME}
#    SOURCES_FILES ${SOURCES_FILES}
#    HEADER_FILES ${HEADER_FILES}
#    LIBS ${OpenCV_LIBS}
#    INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS}
#    DEFINITIONS OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR}
#    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
#    INSTALL_DIR ${INSTALL_PATH}
#    VERSION ${BUILD_VERSION})

# Install protobuf
find_package(protobuf)
if(protobuf_FOUND)
    INSTALL_TARGETS(TARGETS protobuf::libprotobuf-lite protobuf::libprotobuf protobuf::libprotoc protobuf::protoc)
endif()

# Install model files
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv")
    if(ENABLE_DOWNLOAD)
        DOWNLOAD(FILENAME "deploy.prototxt"
            HASH 6e1471d86330a9e7f9f5738318ede387
            URL "https://github.com/opencv/opencv/raw/master/samples/dnn/face_detector/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
            STATUS RETVAL RELATIVE_URL)
        DOWNLOAD(FILENAME "res10_300x300_ssd_iter_140000_fp16.caffemodel"
            HASH f737f886e33835410c69e3ccfe0720a1
            URL "https://github.com/opencv/opencv_3rdparty/raw/dnn_samples_face_detector_20180205_fp16/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
            STATUS RETVAL RELATIVE_URL)
        DOWNLOAD(FILENAME "opencv_face_detector.pbtxt"
            HASH 6121c73b825f731d6d184220b67bd2e9
            URL "https://github.com/opencv/opencv/raw/master/samples/dnn/face_detector/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
            STATUS RETVAL RELATIVE_URL)
        DOWNLOAD(FILENAME "opencv_face_detector_uint8.pb"
            HASH 56acf81f55d9b9e96c3347bc65409b9e
            URL "https://github.com/opencv/opencv_3rdparty/raw/dnn_samples_face_detector_20180220_uint8/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
            STATUS RETVAL RELATIVE_URL)
    else()
        message(AUTHOR_WARNING "Please set ENABLE_DOWNLOAD to ON "
            " to automation download, or manual download Opencv from"
            " https://github.com/opencv/opencv , then download model files from"
            " [opencv source directory]/samples/dnn/face_detector "
            " execute download_weights.py to downlaod model files and copy"
            " there to ${CMAKE_SOURCE_DIR}/model/Opencv")
    endif()
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv")
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Opencv/deploy.prototxt
        ${CMAKE_SOURCE_DIR}/model/Opencv/res10_300x300_ssd_iter_140000_fp16.caffemodel
        ${CMAKE_SOURCE_DIR}/model/Opencv/opencv_face_detector.pbtxt
        ${CMAKE_SOURCE_DIR}/model/Opencv/opencv_face_detector_uint8.pb
        DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
        COMPONENT Runtime)
endif()
