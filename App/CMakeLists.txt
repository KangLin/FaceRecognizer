
project(FaceRecognizerApp
    DESCRIPTION "Face recognizer"
    HOMEPAGE_URL "https://github.com/KangLin/FaceRecognizer"
)

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
    FrmBrowseFile.cpp
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
    FrmBrowseFile.h
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
    FrmBrowseFile.ui
    )

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

# Install other files
set(OTHER_FILES
    ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog_zh_CN.md)
INSTALL_FILE(SOURCES ${OTHER_FILES}
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Runtime)

IF(WIN32)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/FaceRecognizer.ico
        DESTINATION .
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Install/Install.nsi
        DESTINATION "${CMAKE_BINARY_DIR}"
            COMPONENT Runtime)
ENDIF()

if(UNIX AND NOT ANDROID)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/share/FaceRecognizer.desktop
        DESTINATION "share/applications"
            COMPONENT Runtime)
    INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/share/FaceRecognizer.sh
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/FaceRecognizer.png
        DESTINATION "share/pixmaps"
            COMPONENT Runtime)
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/etc/FaceRecognizer_logqt.ini)
    configure_file(${CMAKE_SOURCE_DIR}/etc/FaceRecognizer_logqt.ini
        ${CMAKE_BINARY_DIR}/etc/FaceRecognizer_logqt.ini
        COPYONLY)
endif()
INSTALL_FILE(SOURCES ${CMAKE_SOURCE_DIR}/etc/FaceRecognizer_logqt.ini
    DESTINATION etc
    COMPONENT Runtime)

INSTALL_ICON_THEME(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/icons)
