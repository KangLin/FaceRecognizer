option(USE_SEETA "Use SeetaFace" ON)
message(STATUS "Use seetaface: ${USE_SEETA}")
if(NOT USE_SEETA)
    return()
endif()

project(PluginFaceSeeta)
find_package(SeetaFace)
message(STATUS "Plugins SeetaFace:${SeetaFace_FOUND};${SeetaFace_LIBRARIES}")
if(NOT SeetaFace_FOUND)
    return()
endif()

INSTALL_TARGETS(TARGETS ${SeetaFace_LIBRARIES})

set(SOURCES_FILES
    FaceSeeta2.cpp
    DetectorSeeta.cpp
    TrackerSeeta.cpp
    LandmarkerSeeta.cpp
    RecognizerSeeta.cpp
    FaceToolsSeeta.cpp
    )

set(HEADER_FILES
    FaceSeeta2.h
    DetectorSeeta.h
    TrackerSeeta.h
    LandmarkerSeeta.h
    RecognizerSeeta.h
    FaceToolsSeeta.h
    )

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
endif()
ADD_PLUGIN_TARGET(NAME ${PROJECT_NAME}
    SOURCE_FILES ${SOURCES_FILES} ${HEADER_FILES}
    PRIVATE_LIBS ${SeetaFace_LIBRARIES} FaceRecognizer
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
    INSTALL_DIR ${INSTALL_PATH}
    INSTALL_RPATH
        "$ORIGIN:$ORIGIN/../../lib:$ORIGIN/../../lib/${CMAKE_LIBRARY_ARCHITECTURE}"
    VERSION ${FaceRecognizer_VERSION})

# Install model files
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/model/Seeta")
    if(ENABLE_DOWNLOAD_MODULE)
        DOWNLOAD(FILENAME "fd_2_00.dat"
            HASH e88669e5f1301ca56162de8aef1fd5d5
            URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
            STATUS retval RELATIVE_URL)
        DOWNLOAD(FILENAME "fr_2_10.dat"
            HASH 2d637aad8b1b7ae62154a877ec291c99
            URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
            STATUS retval RELATIVE_URL)
        DOWNLOAD(FILENAME "pd_2_00_pts5.dat"
            HASH 877a44aa6f07cb3064ad2828f50f261a
            URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
            STATUS retval RELATIVE_URL)
        DOWNLOAD(FILENAME "pd_2_00_pts81.dat"
            HASH f3f812f01121b5a80384af3c35211bdd
            URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
            DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
            STATUS retval RELATIVE_URL)
    else(ENABLE_DOWNLOAD_MODULE)
        message(AUTHOR_WARNING "Please set ENABLE_DOWNLOAD_MODULE to ON "
            " to automation download, or manual download Seeta model files from"
            " https://github.com/KangLin/SeetaFace2/releases/tag/model"
            " to ${CMAKE_SOURCE_DIR}/model/Seeta")
    endif(ENABLE_DOWNLOAD_MODULE)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/model/Seeta")
    INSTALL_FILE(SOURCES ${CMAKE_SOURCE_DIR}/model/Seeta/fd_2_00.dat
            ${CMAKE_SOURCE_DIR}/model/Seeta/fr_2_10.dat
            ${CMAKE_SOURCE_DIR}/model/Seeta/pd_2_00_pts5.dat
            ${CMAKE_SOURCE_DIR}/model/Seeta/pd_2_00_pts81.dat
        DESTINATION ${INSTALL_DATA_PREFIX}/model/Seeta
            COMPONENT Runtime)
endif()
