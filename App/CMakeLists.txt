cmake_minimum_required(VERSION 2.8)

project(FaceRecognizer)

find_package(SeetaFace
    REQUIRED COMPONENTS 
        SeetaNet
        SeetaFaceDetector
        SeetaFaceLandmarker
        SeetaFaceRecognizer)

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
if(ANDROID)
    add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
endif()
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
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(${CMAKE_CXX_COMPILER} MATCHES ".*/(g\\+\\+)(\\..*)")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-mwindows")
elseif(${CMAKE_CXX_COMPILER} MATCHES ".*/(cl)(\\..*)")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE
        "/SUBSYSTEM:WINDOWS\",5.01\" /ENTRY:mainCRTStartup")
endif()

if(ANDROID)
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            )
else()
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            )
endif()
IF(WIN32)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/image/FaceRecognizer.ico DESTINATION .)
ENDIF()
INSTALL(FILES $<TARGET_FILE:SeetaNet>
    $<TARGET_FILE:SeetaFaceDetector>
    $<TARGET_FILE:SeetaFaceLandmarker>
    $<TARGET_FILE:SeetaFaceRecognizer>
    DESTINATION "${CMAKE_INSTALL_BINDIR}")

IF(ANDROID)
    Set(JSON_FILE ${CMAKE_BINARY_DIR}/android_deployment_settings.json)
    GENERATED_DEPLOYMENT_SETTINGS(NAME ${JSON_FILE}
        APPLACTION "${CMAKE_BINARY_DIR}/bin/lib${PROJECT_NAME}.so")

    set(ANDROID_DELOP_PARA --gradle)
    if(Qt5_VERSION VERSION_LESS 5.8.0)
        find_program(ANT NAMES ant) # PATHS  "/usr/bin")
        MESSAGE("ant:${ANT}\n")
        if(ANT_FOUND)
            set(ANDROID_DELOP_PARA --ant ${ANT})
        endif()
    endif()
    
    add_custom_target(APK #TARGET ${PROJECT_NAME} POST_BUILD
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}
        #COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:${PROJECT_NAME}>/*" ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
            --output ${CMAKE_BINARY_DIR}/android-build
            --input ${JSON_FILE}
            --verbose
            ${ANDROID_DELOP_PARA}
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
