project(PlugFaceNcnn)
find_package(ncnn)
message("== Plugs ncnn:${ncnn_FOUND}")
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

if(EXISTS "${CMAKE_SOURCE_DIR}/model/ncnn")
    INSTALL(DIRECTORS ${CMAKE_SOURCE_DIR}/model/ncnn
        DESTINATION ${INSTALL_DATA_PREFIX}/model
            COMPONENT Runtime)
endif()

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
endif()
ADD_PLUG_TARGET(NAME ${PROJECT_NAME}
    SOURCES_FILES ${SOURCES_FILES}
    HEADER_FILES ${HEADER_FILES}
    LIBS ncnn
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
    INSTALL_DIR ${INSTALL_PATH}
    VERSION ${BUILD_VERSION})
