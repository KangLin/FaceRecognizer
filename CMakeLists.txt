# Author: Kang Lin <kl222@126.com>

cmake_minimum_required(VERSION 2.8)

project(FaceRecognizer
    HOMEPAGE_URL "https://github.com/KangLin/FaceRecognizer"
    )

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
if("Debug" STREQUAL CMAKE_BUILD_TYPE)
    add_definitions(-D_DEBUG)
endif()

OPTION(ENABLE_DOWNLOAD_MODULE "Enable automation download module files" OFF)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

#需要的QT组件
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
SET(QT_COMPONENTS Core Gui Widgets Xml Network Multimedia Sql)
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    set(CMAKE_CXX_STANDARD 17)
    LIST(APPEND QT_COMPONENTS MultimediaWidgets Core5Compat)
else()
    set(CMAKE_CXX_STANDARD 11)
    if(ANDROID)
        LIST(APPEND QT_COMPONENTS AndroidExtras)
    endif()
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 5.10.0 AND ANDROID)
    message(FATAL_ERROR "Qt must great 5.10.0")
endif()
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        SET(QT_LIBRARIES ${QT_LIBRARIES} Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    add_compile_options(-Wno-sign-compare -Wno-unused-parameter)
    
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-g -ggdb)
    else()
        add_compile_options(-O3)
    endif()
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
else(BUILD_SHARED_LIBS)
    add_definitions(-DQT_STATICPLUGIN)
endif(BUILD_SHARED_LIBS)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(${CMAKE_SOURCE_DIR}/cmake/Download.cmake)

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

if(NOT RabbitCommon_DIR)
    set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
    if(NOT RabbitCommon_DIR)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    message(STATUS "Use RabbitCommon source code")
    add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    find_package(RabbitCommon)
    if(NOT RabbitCommon_FOUND)
        message(FATAL_ERROR
                "RabbitCommon_DIR is not found. Please use one of the following ways to set it:\n"
                "1. Set RabbitCommon_DIR to the install prefix of RabbitCommon.\n"
                "2. Set RabbitCommon_DIR to source code root of RabbitCommon.\n"
                "2.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon\n"
                "   ag:\n"
                "       git clone https://github.com/KangLin/RabbitCommon.git\n"
                "2.2 Then set cmake variable or environment variable RabbitCommon_DIR to download root directory.\n"
                "   ag:\n"
                "       cmake -DRabbitCommon_DIR=")
    endif()
endif()

GET_VERSION(OUT_VERSION FaceRecognizer_VERSION
    OUT_REVISION FaceRecognizer_REVISION)
if(NOT FaceRecognizer_VERSION)
    SET(FaceRecognizer_VERSION "v0.0.4")
endif()
message("FaceRecognizer_VERSION:${FaceRecognizer_VERSION};FaceRecognizer_REVISION:${FaceRecognizer_REVISION}")
set(VERSION ${FaceRecognizer_VERSION})

# Set install data prefix
set(INSTALL_DATA_PREFIX ${CMAKE_INSTALL_DATADIR})
if(ANDROID)
    set(INSTALL_FACE_RECOGNIZER_PLUGS_DIR "libs/${CMAKE_ANDROID_ARCH_ABI}")
else()
    set(INSTALL_FACE_RECOGNIZER_PLUGS_DIR plugins)
endif()
set(FACE_RECOGNIZER_PLUGS_BINARY_DIR ${CMAKE_BINARY_DIR}/plugins)

add_subdirectory(Src)
option(BUILD_APP "Build applaction" ON)
if(BUILD_APP)
    add_subdirectory(App)
endif(BUILD_APP)

# 产生文档
option(BUILD_DOCS "Generating API documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Doxygen)
        file(COPY ${CMAKE_SOURCE_DIR}/docs/Doxygen/index.html
            DESTINATION ${CMAKE_BINARY_DIR}/Doxygen)
        foreach(OUTPUT_LANGUAGE English Chinese)
            set(PROJECT_NAME "Face recognizer")
            if(OUTPUT_LANGUAGE STREQUAL "Chinese")
                set(PROJECT_NAME "人脸识别")
            endif()
            configure_file(${CMAKE_SOURCE_DIR}/docs/Doxygen/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile_${OUTPUT_LANGUAGE} @ONLY)
            add_custom_target(doc_${OUTPUT_LANGUAGE} ALL
                ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile_${OUTPUT_LANGUAGE} # &> doxygen.log
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen" VERBATIM
                )
        endforeach()
        install(DIRECTORY ${CMAKE_BINARY_DIR}/Doxygen/
            DESTINATION ${CMAKE_INSTALL_DOCDIR}/${OUTPUT_LANGUAGE}
                COMPONENT Runtime)
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCS)

message(STATUS "Build shared library: ${BUILD_SHARED_LIBS}")
message(STATUS "Build arch: ${BUILD_ARCH}")
message(STATUS "Build application: ${BUILD_APP}")
message(STATUS "Build automation download: ${ENABLE_DOWNLOAD_MODULE}")
message(STATUS "Build documents: ${BUILD_DOCS}")
