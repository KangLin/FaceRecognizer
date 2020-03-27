PROJECT(PlugOpencvFace)

set(SOURCES_FILES
    DetectorOpenCV.cpp
    LandmarkerOpenCV.cpp
    RecognizerOpenCV.cpp
    FaceOpenCV.cpp
    )
set(HEADER_FILES
    DetectorOpenCV.h
    LandmarkerOpenCV.h
    RecognizerOpenCV.h
    FaceOpenCV.h
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
