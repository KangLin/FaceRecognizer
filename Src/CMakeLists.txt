cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(FaceRecognizer)

# Set install data prefix
if(ANDROID)
    set(INSTALL_DATA_PREFIX assets/data)
else()
    set(INSTALL_DATA_PREFIX data)
endif()
    
OPTION(HAVE_PERFORMANCE "Set to ON to build performance)." OFF)
if(HAVE_PERFORMANCE)
    set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_PERFORMANCE)
    set(SOURCES_FILES ${SOURCES_FILES} Performance.cpp)
endif()

option(USE_YUV "Use libyuv" ON)
if(USE_YUV)
    #Use: https://github.com/KangLin/libyuv
    find_package(YUV)
    if(YUV_FOUND)
        set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_LIBYUV)
        set(LIBS ${LIBS} YUV)
        if(ANDROID)
            INSTALL(FILES $<TARGET_FILE:YUV>
                    DESTINATION "libs/${ANDROID_ABI}")
        else()
            INSTALL(FILES $<TARGET_FILE:YUV>
                    DESTINATION "${CMAKE_INSTALL_BINDIR}")
        endif()
    endif()
endif(USE_YUV)

option(USE_FFMPEG "Use ffmpeg" ON)
if(USE_FFMPEG)
    find_package(FFMPEG)
endif(USE_FFMPEG)

option(USE_OPENCV "Use opencv" ON)
if(USE_OPENCV)
    find_package(OpenCV)
    if(OpenCV_FOUND)
        set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_OPENCV OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR})
        set(LIBS ${LIBS} ${OpenCV_LIBS})
        set(INCLUDE_DIRS ${INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
        message("OpenCV_LIBS:${OpenCV_LIBS}")

        foreach(opencv_component ${OpenCV_LIBS})
            if(ANDROID)
                INSTALL(FILES $<TARGET_FILE:${opencv_component}>
                    DESTINATION "libs/${ANDROID_ABI}"
                    COMPONENT Runtime)
            else()
                INSTALL(FILES $<TARGET_FILE:${opencv_component}>
                    DESTINATION "${CMAKE_INSTALL_BINDIR}"
                    COMPONENT Runtime)
            endif()
            
            if(opencv_component STREQUAL "opencv_dnn")
                set(SOURCES_FILES
                    ${SOURCES_FILES}
                    OpenCV/DetectorOpenCVDNN.cpp
                    )
                set(HEADER_FILES
                    ${HEADER_FILES}
                    OpenCV/DetectorOpenCVDNN.h
                    )
                # Install model files
                if(EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv")
                    INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Opencv/deploy.prototxt
                            ${CMAKE_SOURCE_DIR}/model/Opencv/res10_300x300_ssd_iter_140000_fp16.caffemodel
                            ${CMAKE_SOURCE_DIR}/model/Opencv/opencv_face_detector.pbtxt
                            ${CMAKE_SOURCE_DIR}/model/Opencv/opencv_face_detector_uint8.pb
                        DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
                        COMPONENT Runtime)
                else()
                    message(AUTHOR_WARNING "Please download Opencv from"
                        " https://github.com/opencv/opencv , then download model files from"
                        " [opencv source directory]/samples/dnn/face_detector "
                        " execute download_weights.py to downlaod model files and copy"
                        " there to ${CMAKE_SOURCE_DIR}/model/Opencv")
                endif()
            endif()
            
            set(SOURCES_FILES
                ${SOURCES_FILES}
                OpenCV/LandmarkerOpenCV.cpp
                )
            set(HEADER_FILES
                ${HEADER_FILES}
                OpenCV/LandmarkerOpenCV.h
                )
        endforeach()
        
        set(SOURCES_FILES
            ${SOURCES_FILES}
            OpenCV/FaceOpenCV.cpp
            )
        set(HEADER_FILES
            ${HEADER_FILES}
            OpenCV/FaceOpenCV.h
            )
        
        # Install model files
        if(EXISTS "${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml")
            INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Opencv/lbfmodel.yaml
                DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
                COMPONENT Runtime)
        else()
            if(AUTO_DOWNLOAD)
                DOWNLOAD(FILENAME "lbfmodel.yaml"
                    HASH 3521fd7e2a0eb3d14634ec640e8b430d
                    URL "https://raw.githubusercontent.com/kurnianggoro/GSOC2017/master/data/lbfmodel.yaml"
                    DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Opencv"
                    STATUS RETVAL)
            else(AUTO_DOWNLOAD)
                message(AUTHOR_WARNING "Please download Opencv from"
                    " https://raw.githubusercontent.com/kurnianggoro/GSOC2017/master/data/lbfmodel.yaml "
                    " to ${CMAKE_SOURCE_DIR}/model/Opencv")
            endif(AUTO_DOWNLOAD)
        endif()
        
        set(MODEL_FILES ${OpenCV_DIR}/etc)
        if(ANDROID)
            set(MODEL_FILES ${OpenCV_DIR}/../../etc)
        endif(ANDROID)
        INSTALL(DIRECTORY ${MODEL_FILES}/
            DESTINATION ${INSTALL_DATA_PREFIX}/model/Opencv
            COMPONENT Runtime)
    endif()
endif(USE_OPENCV)

find_package(SeetaFace)
if(SeetaFace_FOUND)
    set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_SEETA_FACE)
    set(LIBS ${LIBS} ${SeetaFace_LIBRARIES})
    foreach(componet_seetaFace ${SeetaFace_LIBRARIES})
        if(ANDROID)
            INSTALL(FILES $<TARGET_FILE:${componet_seetaFace}>
                DESTINATION "libs/${ANDROID_ABI}"
                COMPONENT Runtime)
        else()
            INSTALL(FILES $<TARGET_FILE:${componet_seetaFace}>
                DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime)
        endif()
    endforeach()
    
    set(SOURCES_FILES
        ${SOURCES_FILES}
        Seeta/FaceSeeta.cpp
        Seeta/DetectorSeeta.cpp
        Seeta/TrackerSeeta.cpp
        Seeta/LandmarkerSeeta.cpp
        Seeta/RecognizerSeeta.cpp
        Seeta/FaceToolsSeeta.cpp
        )
    
    set(HEADER_FILES
        ${HEADER_FILES}
        Seeta/FaceSeeta.h
        Seeta/DetectorSeeta.h
        Seeta/TrackerSeeta.h
        Seeta/LandmarkerSeeta.h
        Seeta/RecognizerSeeta.h
        Seeta/FaceToolsSeeta.h
        )
    
    set(SOURCE_UI_FILES
        ${SOURCE_UI_FILES}
        )
    
    # Install model files
    if(EXISTS "${CMAKE_SOURCE_DIR}/model/Seeta")
        INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Seeta/fd_2_00.dat
                ${CMAKE_SOURCE_DIR}/model/Seeta/fr_2_10.dat
                ${CMAKE_SOURCE_DIR}/model/Seeta/pd_2_00_pts5.dat
                ${CMAKE_SOURCE_DIR}/model/Seeta/pd_2_00_pts81.dat
            DESTINATION ${INSTALL_DATA_PREFIX}/model/Seeta
            COMPONENT Runtime)
    else()
        if(AUTO_DOWNLOAD)
            DOWNLOAD(FILENAME "fd_2_00.dat"
                HASH e88669e5f1301ca56162de8aef1fd5d5
                URL "https://github.com/KangLin/SeetaFace2/releases/tag/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
            DOWNLOAD(FILENAME "fr_2_10.dat"
                HASH 2d637aad8b1b7ae62154a877ec291c99
                URL "https://github.com/KangLin/SeetaFace2/releases/tag/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
            DOWNLOAD(FILENAME "pd_2_00_pts5.dat"
                HASH 877a44aa6f07cb3064ad2828f50f261a
                URL "https://github.com/KangLin/SeetaFace2/releases/tag/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
            DOWNLOAD(FILENAME "pd_2_00_pts81.dat"
                HASH f3f812f01121b5a80384af3c35211bdd
                URL "https://github.com/KangLin/SeetaFace2/releases/tag/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
        else(AUTO_DOWNLOAD)
            message(AUTHOR_WARNING "Please download Seeta model files from"
                " https://github.com/KangLin/SeetaFace2/releases/tag/model"
                " to ${CMAKE_SOURCE_DIR}/model/Seeta")
        endif(AUTO_DOWNLOAD)
    endif()
endif()

set(SOURCES_FILES
    ${SOURCES_FILES}
    CameraQtCaptureVideoFrame.cpp
    ImageTool.cpp
    Log.cpp
    ParameterFactory.cpp
    Parameter.cpp
    ParameterDetector.cpp
    ParameterLandmark.cpp
    ParameterRecognizer.cpp
    ParameterFaceTools.cpp
    FactoryFace.cpp
    Face.cpp
    Detector.cpp
    Tracker.cpp
    Landmarker.cpp
    Recognizer.cpp
    FaceTools.cpp
    database/Database.cpp
    database/DataRegister.cpp
    database/TableRegister.cpp
    )

set(INSTALLHEADER_FILES
    ImageTool.h
    CameraQtCaptureVideoFrame.h
    FactoryFace.h
    Face.h
    Detector.h
    Tracker.h
    Landmarker.h
    Recognizer.h
    FaceTools.h
    Performance.h
    ParameterFactory.h
    Parameter.h
    ParameterDetector.h
    ParameterLandmark.h
    ParameterRecognizer.h
    ParameterFaceTools.h
    )

set(HEADER_FILES
    ${HEADER_FILES}
    ${INSTALLHEADER_FILES}
    Log.h
    database/Database.h
    database/DataRegister.h
    database/TableRegister.h
    )

#翻译
SET(TRANSLATIONS_NAME ${PROJECT_NAME})
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)

SET(RCC_FILES
    ../Resource/Resource.qrc
    ${TRANSLATIONS_RESOURCE_FILES})

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(BUILD_TYPE STREQUAL "debug")
    set(LIBS_DEFINES ${LIBS_DEFINES} DEBUG)
    SET(RCC_FILES ${RCC_FILES} ../Resource/ResourceSql.qrc)
endif()

add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${RCC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    PUBLIC_HEADER "${INSTALLHEADER_FILES}" # Install head files
    VERSION ${BUILD_VERSION})

target_compile_definitions(${PROJECT_NAME} PUBLIC RABBITCOMMON ${LIBS_DEFINES})
target_link_libraries(${PROJECT_NAME} PUBLIC RabbitCommon ${QT_LIBRARIES} ${LIBS})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR}
    ${INCLUDE_DIRS})

if(BUILD_VERSION)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})
endif()

GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(INSTALLHEADER_FILES ${INSTALLHEADER_FILES} 
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

if(FFMPEG_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ${FFMPEG_DEFINITIONS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${FFMPEG_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${FFMPEG_INCLUDE_DIRS})
endif()

if(ANDROID)
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "libs/${ANDROID_ABI}"
                COMPONENT Runtime
            LIBRARY DESTINATION "libs/${ANDROID_ABI}"
                COMPONENT Runtime
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FaceReconginzer
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )    
else()
    # Install target
    if(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
            LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FaceReconginzer
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    else()
        INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
                COMPONENT Runtime
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FaceReconginzer
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    endif()
endif()

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

# Install sql file
INSTALL(FILES ../Resource/db/database.sql
    DESTINATION ${INSTALL_DATA_PREFIX}/db
    COMPONENT Runtime)

IF("Release" STREQUAL CMAKE_BUILD_TYPE AND WIN32 AND BUILD_SHARED_LIBS)
    IF(CMAKE_BUILD_TYPE MATCHES Release AND MINGW)
        #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND strip "$<TARGET_FILE:${PROJECT_NAME}>"
            )
    ENDIF()

    #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中  
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
        --compiler-runtime
        --verbose 7
        "$<TARGET_FILE:${PROJECT_NAME}>"
        )
    #INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/" DESTINATION "${CMAKE_INSTALL_BINDIR}")
ENDIF()

message("== HAVE_PERFORMANCE:${HAVE_PERFORMANCE}")
message("== OpenCV:${OpenCV_FOUND}")
message("== FFMPGE:${FFMPEG_FOUND}")
message("== LIBYUV:${YUV_FOUND}")
message("== SeetaFac:${SeetaFace_FOUND}")
