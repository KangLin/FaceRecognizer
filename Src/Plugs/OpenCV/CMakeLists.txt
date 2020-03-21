PROJECT(PlugOpenCV)

option(USE_OPENCV "Use opencv" ON)
if(NOT USE_OPENCV)
    return()
endif()
find_package(OpenCV)
message("== Plug OpenCV:${OpenCV_FOUND}")
if(NOT OpenCV_FOUND)
    return()
endif()

set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_OPENCV OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR})
set(LIBS ${OpenCV_LIBS})
set(INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
#message("OpenCV_LIBS:${OpenCV_LIBS}")

if(OpenCV_VERSION VERSION_LESS "3.4.0")
    return()
endif()

set(SOURCES_FILES
    )
set(HEADER_FILES
    )

foreach(opencv_component ${OpenCV_LIBS})
    if(ANDROID)
        INSTALL(FILES $<TARGET_FILE:${opencv_component}>
            DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime)
    else(ANDROID)
        INSTALL(FILES $<TARGET_FILE:${opencv_component}>
            DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime)
    endif(ANDROID)
    
    if(opencv_component STREQUAL "opencv_dnn")
        set(SOURCES_FILES
            DetectorOpenCVDNN.cpp
            FaceOpenCVDNN.cpp
            )
        set(HEADER_FILES
            DetectorOpenCVDNN.h
            FaceOpenCVDNN.h
            )
        
        add_library(${PROJECT_NAME}DNN ${SOURCES_FILES} ${HEADER_FILES})
        set_target_properties(${PROJECT_NAME}DNN PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
            ARCHIVE_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
            RUNTIME_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
            VERSION ${BUILD_VERSION})
        target_link_libraries(${PROJECT_NAME}DNN PUBLIC FaceRecognizer RabbitCommon ${QT_LIBRARIES} ${LIBS})
        target_include_directories(${PROJECT_NAME}DNN
            PRIVATE ${RabbitCommon_DIR}/Src
            ${CMAKE_SOURCE_DIR}/Src
            ${CMAKE_BINARY_DIR}
            ${INCLUDE_DIRS})
        
        if(BUILD_VERSION)
            target_compile_definitions(${PROJECT_NAME}DNN PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
        endif()
        
        # Install target
        INSTALL(TARGETS ${PROJECT_NAME}DNN
            RUNTIME DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
            COMPONENT Runtime
            LIBRARY DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
            COMPONENT Runtime
            #ARCHIVE DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
            )
        
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
        
    endif(opencv_component STREQUAL "opencv_dnn")
    
    if(opencv_component STREQUAL "opencv_face")
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
        
    endif(opencv_component STREQUAL "opencv_face")
endforeach()
