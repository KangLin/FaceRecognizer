cmake_minimum_required(VERSION 2.8)

project(FaceRecognizer)

find_package(SeetaNet REQUIRED)
find_package(SeetaFaceDetector REQUIRED)
find_package(SeetaFaceLandmarker REQUIRED)
find_package(SeetaFaceRecognizer REQUIRED)

set(SOURCES_FILES
    main.cpp
    MainWindow.cpp    
    FrmDisplay.cpp
    FrmRegister.cpp
    FrmRecognizer.cpp
    CameraQtCaptureVideoFrame.cpp
    )

set(HEADER_FILES
    MainWindow.h    
    FrmDisplay.h
    FrmRegister.h
    FrmRecognizer.h
    CameraQtCaptureVideoFrame.h
    )

set(SOURCE_UI_FILES
    MainWindow.ui
    FrmDisplay.ui
    FrmRegister.ui
    FrmRecognizer.ui
    )


#翻译
SET(TRANSLATIONS_NAME ${PROJECT_NAME})
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)

add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")

SET(RCC_FILES
    ../Resource/Resource.qrc
    )
if(WIN32)
    SET(RCC_FILES ${RCC_FILES} AppIcon.rc)
endif()
add_executable(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
target_link_libraries(${PROJECT_NAME}
    ${QT_LIBRARIES}
    RabbitCommon
    SeetaNet
    SeetaFaceDetector
    SeetaFaceLandmarker
    SeetaFaceRecognizer)
target_compile_definitions(${PROJECT_NAME}
                    PRIVATE -DRABBITCOMMON)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
IF(WIN32)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/image/FaceRecognizer.ico DESTINATION .)
ENDIF()
INSTALL(FILES $<TARGET_FILE:SeetaNet>
    $<TARGET_FILE:SeetaFaceDetector>
    $<TARGET_FILE:SeetaFaceLandmarker>
    $<TARGET_FILE:SeetaFaceRecognizer>
    DESTINATION "${CMAKE_INSTALL_BINDIR}")
IF(ANDROID)
    include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)
    GENERATED_DEPLOYMENT_SETTINGS()
    find_program(ANT NAMES ant) # PATHS  "/usr/bin")
    MESSAGE("ant:${ANT}\n")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_BINARY_DIR}/libFaceRecognizer.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/lib${PROJECT_NAME}.so
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
            --input ${PROJECT_BINARY_DIR}/android-libFaceRecognizer.so-deployment-settings.json
            --output ${PROJECT_BINARY_DIR}/android-build
            --verbose
            #--ant /usr/bin/ant
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)

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
    INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/" DESTINATION "${CMAKE_INSTALL_BINDIR}")
ENDIF()
