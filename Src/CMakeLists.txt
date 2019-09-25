cmake_minimum_required(VERSION 2.8)

project(FaceRecognizer)

find_package(LIBYUV)
find_package(FFMPEG)
find_package(SeetaFace)

set(SOURCES_FILES
    FrmDisplay.cpp
    CameraQtCaptureVideoFrame.cpp
    ImageTool.cpp
    Log.cpp
    yuv2rgb/yuv2rgb.cpp
    yuv2rgb/bmp_utils.cpp
    yuv2rgb/rgb2yuv.cpp
    )

set(HEADER_FILES 
    FrmDisplay.h
    CameraQtCaptureVideoFrame.h
    ImageTool.h
    Log.h
    yuv2rgb/yuv2rgb.h
    yuv2rgb/bmp_utils.h
    yuv2rgb/rgb2yuv.h
    )

set(SOURCE_UI_FILES
    FrmDisplay.ui
    )

if(SeetaFace_FOUND)
    set(SOURCES_FILES
        ${SOURCES_FILES}
        FrmRegister.cpp
        FrmRecognizer.cpp
        )
    
    set(HEADER_FILES
        ${HEADER_FILES}
        FrmRegister.h
        FrmRecognizer.h
        )
    
    set(SOURCE_UI_FILES
        ${SOURCE_UI_FILES}
        FrmRegister.ui
        FrmRecognizer.ui
        )
endif()

#翻译
SET(TRANSLATIONS_NAME ${PROJECT_NAME})
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)

SET(RCC_FILES
    ../Resource/Resource.qrc
    ${TRANSLATIONS_RESOURCE_FILES})

add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_compile_definitions(${PROJECT_NAME} PUBLIC RABBITCOMMON)
target_link_libraries(${PROJECT_NAME} PUBLIC RabbitCommon ${QT_LIBRARIES})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR})

GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(INSTALLHEADER_FILES ${INSTALLHEADER_FILES} 
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})
# Install head files
set_property(
    TARGET
        ${PROJECT_NAME}
    PROPERTY
        PUBLIC_HEADER "${INSTALLHEADER_FILES}"
)

if(SeetaFace_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC HAVE_SEETA_FACE)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        SeetaNet
        SeetaFaceDetector
        SeetaFaceLandmarker
        SeetaFaceRecognizer)
    
    if(ANDROID)
        if(SeetaFace_FOUND)
            INSTALL(FILES $<TARGET_FILE:SeetaNet>
                $<TARGET_FILE:SeetaFaceDetector>
                $<TARGET_FILE:SeetaFaceLandmarker>
                $<TARGET_FILE:SeetaFaceRecognizer>
                DESTINATION "libs/${ANDROID_ABI}")
        endif()
    else()
        if(SeetaFace_FOUND)
            INSTALL(FILES $<TARGET_FILE:SeetaNet>
                $<TARGET_FILE:SeetaFaceDetector>
                $<TARGET_FILE:SeetaFaceLandmarker>
                $<TARGET_FILE:SeetaFaceRecognizer>
                DESTINATION "${CMAKE_INSTALL_BINDIR}")
        endif()
    endif()
endif()

if(LIBYUV_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ${LIBYUV_DEFINITIONS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBYUV_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${LIBYUV_INCLUDE_DIRS})
endif()

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
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FaceReconginzer
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
else()
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FaceReconginzer
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
endif()

export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
)
# Install cmake configure files
install(
        EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")


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

message("== FFMPGE:${FFMPEG_FOUND}")
message("== LIBYUV:${LIBYUV_FOUND}")
message("== SeetaFac:${SeetaFace_FOUND}")
