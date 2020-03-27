PROJECT(PlugOpencvDNN)

set(SOURCES_FILES
    DetectorOpenCVDNN.cpp
    FaceOpenCVDNN.cpp
    )
set(HEADER_FILES
    DetectorOpenCVDNN.h
    FaceOpenCVDNN.h
    )

add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES})
add_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBS_DEFINES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
    VERSION ${BUILD_VERSION})
target_link_libraries(${PROJECT_NAME} PUBLIC FaceRecognizer RabbitCommon ${QT_LIBRARIES} ${LIBS})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_SOURCE_DIR}/Src
    ${CMAKE_BINARY_DIR}
    ${INCLUDE_DIRS})

if(BUILD_VERSION)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
endif()

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    COMPONENT Runtime
    LIBRARY DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    COMPONENT Runtime
    #ARCHIVE DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    )

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Config
    RUNTIME DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    COMPONENT Runtime
    LIBRARY DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    COMPONENT Runtime
    ARCHIVE DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    )

export(TARGETS ${PROJECT_NAME}
    APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    )

# Install cmake configure files
install(EXPORT ${PROJECT_NAME}Config
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
    )
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")


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
