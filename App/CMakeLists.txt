cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(FaceRecognizerApp)

set(SOURCE_FILES
    FrmDisplay.cpp
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
    MainWindow.ui
    FrmRegisterImage.ui
    FrmRecognizerImage.ui
    FrmRegisterVideo.ui
    FrmRecognizerVideo.ui
    ManageRecognizerVideo.ui
    FrmPara.ui
    FrmBroweFile.ui
    )


add_definitions(-DBUILD_VERSION="${FaceRecognizer_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")

SET(RCC_FILES
    Resource/Resource.qrc
    ${TRANSLATIONS_QRC_FILES})
if(WIN32)
    SET(RCC_FILES ${RCC_FILES} AppIcon.rc)
endif()

ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    ISWINDOWS
    VERSION ${FaceRecognizer_VERSION}
    PRIVATE_DEFINITIONS FaceRecognizer_VERSION="${FaceRecognizer_VERSION}"
        FaceRecognizer_REVISION="${FaceRecognizer_REVISION}"
    SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES}
    PRIVATE_LIBS ${QT_LIBRARIES} FaceRecognizer)

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

if(Log4Qt_FOUND)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/etc/FaceRecognizer_log4qt.conf)
        configure_file(${CMAKE_SOURCE_DIR}/etc/FaceRecognizer_log4qt.conf
            ${CMAKE_BINARY_DIR}/etc/FaceRecognizer_log4qt.conf
            COPYONLY)
    endif()
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/etc/FaceRecognizer_log4qt.conf
        DESTINATION etc
            COMPONENT Runtime)
else()
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/etc/FaceRecognizer_logqt.ini)
        configure_file(${CMAKE_SOURCE_DIR}/etc/FaceRecognizer_logqt.ini
            ${CMAKE_BINARY_DIR}/etc/FaceRecognizer_logqt.ini
            COPYONLY)
    endif()
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/etc/FaceRecognizer_logqt.ini
        DESTINATION etc
            COMPONENT Runtime)
endif()
