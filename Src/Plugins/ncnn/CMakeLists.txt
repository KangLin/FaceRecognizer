option(USE_NCNN "Use ncnn" OFF)
message(STATUS "Use ncnn: ${USE_NCNN}")
if(NOT USE_NCNN)
    return()
endif()

project(PluginFaceNcnn)
find_package(ncnn)
message(STATUS "Plugins ncnn:${ncnn_FOUND}")
if(NOT ncnn_FOUND)
    return()
endif()

# 现在ncnn编译出来的是静态库，所以不需要安装
#INSTALL_TARGETS(TARGETS ncnn)

set(SOURCES_FILES
    FaceNcnn.cpp
    DetectorNcnnRetina.cpp
    )

set(HEADER_FILES
    FaceNcnn.h
    DetectorNcnnRetina.h
    )

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
endif()
ADD_PLUGIN_TARGET(NAME ${PROJECT_NAME}
    SOURCE_FILES ${SOURCES_FILES} ${HEADER_FILES}
    PRIVATE_LIBS ncnn FaceRecognizer
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
    INSTALL_DIR ${INSTALL_PATH}
    VERSION ${FaceRecognizer_VERSION})

# Install model files
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/model/ncnn")
    if(ENABLE_DOWNLOAD_MODULE)
        DOWNLOAD(FILENAME "model_ncnn.tar.gz"
            HASH fd4a88dcf16c29fdeadc852eb9776ffb
            URL "https://github.com/KangLin/ncnn_example/releases/download/model/model_ncnn.tar.gz"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/ncnn"
            STATUS retval UNPACK)
    else(ENABLE_DOWNLOAD_MODULE)
        message(AUTHOR_WARNING "Please set ENABLE_DOWNLOAD_MODULE to ON "
            " to automation download, or manual download ncnn model files from"
            " https://github.com/KangLin/ncnn_example/releases/download/model/model_ncnn.tar.gz"
            " to ${CMAKE_SOURCE_DIR}/model/ncnn and tar xzvf model_ncnn.tar.gz")
    endif(ENABLE_DOWNLOAD_MODULE)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/model/ncnn")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/model/ncnn
        DESTINATION ${INSTALL_DATA_PREFIX}/model
            COMPONENT Runtime)
endif()
