PROJECT(PlugFaceOpencvFace)

set(SOURCES_FILES
    DetectorOpenCV.cpp
    LandmarkerOpenCV.cpp
    RecognizerOpenCV.cpp
    FaceOpenCV.cpp
    ${SOURCES_FILES}
    )
set(HEADER_FILES
    DetectorOpenCV.h
    LandmarkerOpenCV.h
    RecognizerOpenCV.h
    FaceOpenCV.h
    ${HEADER_FILES}
    )

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
endif()
ADD_PLUG_TARGET(NAME ${PROJECT_NAME}
    SOURCES_FILES ${SOURCES_FILES}
    HEADER_FILES ${HEADER_FILES}
    LIBS ${OpenCV_LIBS}
    INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS}
    DEFINITIONS OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR}
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
    INSTALL_DIR ${INSTALL_PATH}
    VERSION ${BUILD_VERSION})

# Install model files
set(MODEL_FILES ${OpenCV_DIR}/etc)
if(ANDROID)
    set(MODEL_FILES ${OpenCV_DIR}/../../etc)
endif(ANDROID)
if(EXISTS ${MODEL_FILES})
    INSTALL(DIRECTORY ${MODEL_FILES}/
        DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
        COMPONENT Runtime)
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml")
    if(ENABLE_DOWNLOAD)
        DOWNLOAD(FILENAME "lbfmodel.yaml"
            HASH 96034b93743e1f7f9748797fb8222dd9
            URL "https://raw.githubusercontent.com/kurnianggoro/GSOC2017/master/data/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
            STATUS RETVAL RELATIVE_URL)
    else(ENABLE_DOWNLOAD)
        message(AUTHOR_WARNING "Please set ENABLE_DOWNLOAD to ON "
            " to automation download, or manual download Opencv from"
            " https://raw.githubusercontent.com/kurnianggoro/GSOC2017/master/data/lbfmodel.yaml "
            " to ${CMAKE_SOURCE_DIR}/model/Opencv")
    endif(ENABLE_DOWNLOAD)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml")
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml
        DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
        COMPONENT Runtime)
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv/face_landmark_model.dat")
    if(ENABLE_DOWNLOAD)
        set(__commit_hash "8afa57abc8229d611c4937165d20e2a2d9fc5a12")
        set(__file_hash "7505c44ca4eb54b4ab1e4777cb96ac05")
        DOWNLOAD(
            FILENAME face_landmark_model.dat
            HASH ${__file_hash}
            URL "https://raw.githubusercontent.com/opencv/opencv_3rdparty/${__commit_hash}/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
            STATUS RETVAL RELATIVE_URL
            )
    else(ENABLE_DOWNLOAD)
        message(AUTHOR_WARNING "Please set ENABLE_DOWNLOAD to ON "
            " to automation download, or manual download Opencv from"
            " https://raw.githubusercontent.com/kurnianggoro/GSOC2017/master/data/lbfmodel.yaml "
            " to ${CMAKE_SOURCE_DIR}/model/Opencv")
    endif(ENABLE_DOWNLOAD)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml")
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml
        DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
        COMPONENT Runtime)
endif()
