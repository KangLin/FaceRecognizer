# Author: Kang Lin <kl222@126.com>

project(FaceRecognizer)

set(SOURCE_FILES
    CameraQtCaptureVideoFrame.cpp
    ImageTool.cpp
    ConverFormat.cpp
    FactoryFace.cpp
    Face.cpp
    FaceBase.cpp
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
    ConverFormat.h
    CameraQtCaptureVideoFrame.h
    FactoryFace.h
    Face.h
    FaceBase.h
    Detector.h
    Tracker.h
    Landmarker.h
    Recognizer.h
    FaceTools.h
    Performance.h
    )

set(HEADER_FILES
    ${HEADER_FILES}
    ${INSTALLHEADER_FILES}
    database/Database.h
    database/DataRegister.h
    database/TableRegister.h
    )

OPTION(BUILD_PERFORMANCE "Set to ON to build performance)." OFF)
if(BUILD_PERFORMANCE)
    set(LIBS_DEFINES ${LIBS_DEFINES} BUILD_PERFORMANCE)
    list(APPEND SOURCE_FILES Performance.cpp)
endif()

option(USE_YUV "Use libyuv" ON)
if(USE_YUV)
    #Use: https://github.com/KangLin/libyuv
    find_package(YUV)
    if(YUV_FOUND)
        set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_LIBYUV)
        set(LIBS ${LIBS} YUV)
#        if(ANDROID)
#            INSTALL(FILES $<TARGET_FILE:YUV>
#                    DESTINATION "libs/${ANDROID_ABI}")
#        else()
#            INSTALL(FILES $<TARGET_FILE:YUV>
#                    DESTINATION "${CMAKE_INSTALL_BINDIR}")
#        endif()
    endif()
endif(USE_YUV)

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(BUILD_TYPE STREQUAL "debug")
    set(LIBS_DEFINES ${LIBS_DEFINES} DEBUG)
    SET(RCC_FILES ${RCC_FILES} ../Resource/ResourceSql.qrc)
endif()

add_subdirectory(Plugins)

# Set flags
if(ANDROID)
    if(ANDROID_ARM_NEON)
        list(APPEND FACERECOGNIZER_OPTIONS -mfpu=neon-vfpv4)
    endif()
    
    list(APPEND FACERECOGNIZER_OPTIONS
        -frtti -fexceptions -funsafe-math-optimizations -ftree-vectorize -ffast-math)
endif()

ADD_TARGET(NAME ${PROJECT_NAME}
    VERSION ${FaceRecognizer_VERSION}
    PRIVATE_DEFINITIONS
        FaceRecognizer_VERSION="${FaceRecognizer_VERSION}"
        FaceRecognizer_REVISION="${FaceRecognizer_REVISION}"
    DEFINITIONS RABBITCOMMON ${LIBS_DEFINES}
    SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES} ${RCC_FILES}
    INSTALL_HEADER_FILES "${INSTALLHEADER_FILES}"
    OPTIONS ${FACERECOGNIZER_OPTIONS}
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    LIBS RabbitCommon ${QT_LIBRARIES} ${LIBS}
    )

# Install sql file
INSTALL_FILE(SOURCES ${CMAKE_SOURCE_DIR}/Resource/db/database.sql
    DESTINATION ${INSTALL_DATA_PREFIX}/db
        COMPONENT Runtime)

message(STATUS "LIBYUV:${YUV_FOUND}")
message(STATUS "Build PERFORMANCE:${BUILD_PERFORMANCE}")
