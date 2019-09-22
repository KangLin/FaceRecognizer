### FindLIBYUV.cmake ---
## 
## Author: Kang Lin <kl222@126.com>

# - Try to find LIBYUV
# Once done this will define
#
#  LIBYUV_FOUND - system has LIBYUV
#  LIBYUV_INCLUDE_DIRS - the LIBYUV include directory
#  LIBYUV_LIBRARIES - Link these to use LIBYUV
#  LIBYUV_DEFINITIONS - Compiler switches required for using LIBYUV

#  Redistribution and use is allowed according to the terms of the New
#  BSD license.

include(FindPackageHandleStandardArgs)

find_path(LIBYUV_INCLUDE_DIR
	NAMES libyuv.h
	PATHS ${LIBYUV_DIR} /opt/libyuv /usr /usr/local /opt/local
    PATH_SUFFIXES include
    )

find_library(LIBYUV_LIBRARY
	NAMES yuv
	PATHS ${LIBYUV_DIR} /opt/libyuv /usr /usr/local /opt/local
    PATH_SUFFIXES lib
    )

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBYUV
    LIBYUV_INCLUDE_DIR LIBYUV_LIBRARY)

mark_as_advanced(LIBYUV_INCLUDE_DIR LIBYUV_LIBRARY)

if(LIBYUV_FOUND)
    set(LIBYUV_INCLUDE_DIRS ${LIBYUV_INCLUDE_DIR})
    set(LIBYUV_LIBRARIES ${LIBYUV_LIBRARY})
else()
    message("Don't fonud libyuv. please set -DLIBYUV_DIR=")
endif()
 
