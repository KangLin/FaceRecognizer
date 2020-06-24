cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(FaceRecognizerApp)

set(SOURCES_FILES
    FrmDisplay.cpp
    DlgLog.cpp
    main.cpp
    MainWindow.cpp
    FrmRegisterImage.cpp
    FrmRecognizerImage.cpp
    FrmRegisterVideo.cpp
    ManageRegisterVideo.cpp
    ParameterRegisterImage.cpp
    FrmRecognizerVideo.cpp
    ManageRecognizerVideo.cpp
    RecognizerThread.cpp
    RecognizerVideo.cpp
    FrmPara.cpp
    DelegateParamter.cpp
    FrmBroweFile.cpp
    )

set(HEADER_FILES
    FrmDisplay.h
    DlgLog.h
    MainWindow.h
    FrmRegisterImage.h
    FrmRecognizerImage.h
    FrmRegisterVideo.h
    ManageRegisterVideo.h
    ParameterRegisterImage.h
    FrmRecognizerVideo.h
    ManageRecognizerVideo.h
    RecognizerThread.h
    RecognizerVideo.h
    FrmPara.h
    DelegateParamter.h
    FrmBroweFile.h
    )

set(SOURCE_UI_FILES
    FrmDisplay.ui
    DlgLog.ui
    MainWindow.ui
    FrmRegisterImage.ui
    FrmRecognizerImage.ui
    FrmRegisterVideo.ui
    FrmRecognizerVideo.ui
    ManageRecognizerVideo.ui
    FrmPara.ui
    FrmBroweFile.ui
    )

#翻译
SET(TRANSLATIONS_NAME ${PROJECT_NAME})
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)

add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")

SET(RCC_FILES
    Resource/Resource.qrc
    ${TRANSLATIONS_RESOURCE_FILES})
if(WIN32)
    SET(RCC_FILES ${RCC_FILES} AppIcon.rc)
endif()
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} FaceRecognizer)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    VERSION ${BUILD_VERSION}
    )
if(BUILD_VERSION)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
endif()
if(UNIX AND NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${PROJECT_NAME})
endif()

if(MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")
elseif(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
        "/SUBSYSTEM:WINDOWS\",5.01\" /ENTRY:mainCRTStartup")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Src)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR})

# Install target
if(ANDROID)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
elseif(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
else()
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )        
endif()
# Install syle files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Style/
        DESTINATION ${INSTALL_DATA_PREFIX}/style
            COMPONENT Runtime)
IF(WIN32)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/FaceRecognizer.ico
        DESTINATION .
            COMPONENT Runtime)
ENDIF()

if(UNIX AND NOT ANDROID)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/FaceRecognizer.desktop
        DESTINATION "share/applications"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/FaceRecognizer.png
        DESTINATION "share/pixmaps"
            COMPONENT Runtime)
endif()

IF(ANDROID)
    Set(JSON_FILE ${CMAKE_CURRENT_BINARY_DIR}/android_deployment_settings.json)
    GENERATED_DEPLOYMENT_SETTINGS(NAME ${JSON_FILE}
        APPLACTION "${CMAKE_BINARY_DIR}/bin/lib${PROJECT_NAME}.so")

    add_custom_target(APK #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
            COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
                --output ${CMAKE_INSTALL_PREFIX}
                --input ${JSON_FILE}
                --verbose
                --gradle
                --android-platform ${ANDROID_PLATFORM}
        )
ENDIF(ANDROID)

IF( ("Release" STREQUAL CMAKE_BUILD_TYPE)
    OR ("MinSizeRel" STREQUAL CMAKE_BUILD_TYPE)
    AND WIN32 AND BUILD_SHARED_LIBS )
    IF( ("Release" STREQUAL CMAKE_BUILD_TYPE)
        OR ("MinSizeRel" STREQUAL CMAKE_BUILD_TYPE)
        AND MINGW )
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
    INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Runtime)
ENDIF()
