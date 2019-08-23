cmake_minimum_required(VERSION 2.8)

project(Face)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

SET(BUILD_VERSION "v0.2.4")
# Find Git Version Patch
IF(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    if(NOT GIT)
        SET(GIT $ENV{GIT})
    endif()
    if(NOT GIT)
        FIND_PROGRAM(GIT NAMES git git.exe git.cmd)
    endif()
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} describe --tags
            OUTPUT_VARIABLE GIT_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
	    if(NOT GIT_VERSION)
            EXECUTE_PROCESS(
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND ${GIT} rev-parse --short HEAD
                OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        endif()
        SET(BUILD_VERSION ${GIT_VERSION})
    ENDIF()
ENDIF()
message("BUILD_VERSION:${BUILD_VERSION}")
set(VERSION ${BUILD_VERSION})

if("Debug" STREQUAL CMAKE_BUILD_TYPE)
    add_definitions(-D_DEBUG)
endif()

#需要的QT组件
SET(QT_COMPONENTS Core Gui Widgets Xml Network Multimedia)
find_package(Qt5 COMPONENTS ${QT_COMPONENTS})

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

FOREACH(_COMPONENT ${QT_COMPONENTS})
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()

SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")

IF(MSVC)
    # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
    OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
    MARK_AS_ADVANCED(WIN32_USE_MP)
    IF(WIN32_USE_MP)
        #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_compile_options(/MP)
    ENDIF(WIN32_USE_MP)
    add_compile_options(/utf-8)
ENDIF(MSVC)
IF(MINGW)
    add_compile_options("-std=c++0x")
ENDIF()
SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
if (BUILD_SHARED_LIBS)
	add_definitions(-DBUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_definitions("-fPIC")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)

#CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
#check_include_file("math.h" HAVE_MATH_H)

#check_function_exists("fabs" HAVE_FABS)

set(BUILD_PLATFORM "${CMAKE_SYSTEM_NAME}")
# ----------------------------------------------------------------------------
# Detect compiler and target platform architecture
# ----------------------------------------------------------------------------
if(NOT ANDROID)
    if(X86_64 OR CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BUILD_ARCH x86_64)
    elseif(X86 OR CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(BUILD_ARCH x86)
    endif()
else()
    set(BUILD_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

find_package(SeetaNet REQUIRED)
find_package(SeetaFaceDetector REQUIRED)
find_package(SeetaFaceLandmarker REQUIRED)
find_package(SeetaFaceRecognizer REQUIRED)

set(SRC_FILES
    main.cpp
    MainWindow.cpp    
    FrmDisplay.cpp
    CameraQtCaptureVideoFrame.cpp
    )

set(HEADER_FILES
    MainWindow.h    
    FrmDisplay.h
    CameraQtCaptureVideoFrame.h
    )

set(UI_FILES
    MainWindow.ui
    FrmDisplay.ui
    )

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES} ${UI_FILES})
target_link_libraries(${PROJECT_NAME}
    ${QT_LIBRARIES}
    SeetaNet
    SeetaFaceDetector
    SeetaFaceLandmarker
    SeetaFaceRecognizer)
# 设置安装前缀
SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install path prefix." FORCE)

message(STATUS "== Build shared library: ${BUILD_SHARED_LIBS}")
message(STATUS "== Build arch: ${BUILD_ARCH}")
