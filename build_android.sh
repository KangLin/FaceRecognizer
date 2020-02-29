#!/bin/bash

QT_ROOT=/c/Qt/Qt5.13.2/5.13.2/android_arm64_v8a
export YUV_DIR=/d/Source/libyuv/build_android/install
#export OPENCV_DIR=/c/Users/k/Downloads/android4.9_arm_in_windows/sdk/native/jni
FFMPEG_DIR=
export SeetaFace2_DIR=/d/Source/SeetaFace/build_android/install
if [ -n "$1" ]; then
    Qt5_ROOT=$1
fi
if [ -n "$2" ]; then
    ANDROID_NDK=$1
fi
if [ -z "$ANDROID_NDK" ]; then
    echo "$0 Qt5_ROOT ANDROID_NDK"
    exit -1
fi
if [ -n "$QT_ROOT" ]; then
    PARA="${PARA} -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5
            -DQt5Core_DIR=${QT_ROOT}/lib/cmake/Qt5Core
            -DQt5Gui_DIR=${QT_ROOT}/lib/cmake/Qt5Gui
            -DQt5Widgets_DIR=${QT_ROOT}/lib/cmake/Qt5Widgets
            -DQt5Xml_DIR=${QT_ROOT}/lib/cmake/Qt5Xml
            -DQt5Network_DIR=${QT_ROOT}/lib/cmake/Qt5Network
            -DQt5Multimedia_DIR=${QT_ROOT}/lib/cmake/Qt5Multimedia
            -DQt5Sql_DIR=${QT_ROOT}/lib/cmake/Qt5Sql
            -DQt5LinguistTools_DIR=${QT_ROOT}/lib/cmake/Qt5LinguistTools
            -DQt5AndroidExtras_DIR=${QT_ROOT}/lib/cmake/Qt5AndroidExtras"
fi
if [ -n "$YUV_DIR" ]; then
    PARA="${PARA} -DYUV_DIR=${YUV_DIR}/lib/cmake"
else
    PARA="${PARA} -DUSE_YUV=OFF"
fi
if [ -n "$OPENCV_DIR" ]; then
    PARA="${PARA} -DOpenCV_DIR=${OPENCV_DIR}"
else
    PARA="${PARA} -DUSE_OPENCV=OFF"
fi
if [ -n "$FFMPEG_DIR" ]; then
    PARA="${PARA} -DFFMPEG_DIR=${FFMPEG_DIR}"
else
    PARA="${PARA} -DUSE_FFMPEG=OFF"
fi
if [ -n "$SeetaFace2_DIR" ]; then
    PARA="${PARA} -DSeetaFace_DIR=${SeetaFace2_DIR}/lib/cmake 
            -DSeetaNet_DIR=${SeetaFace2_DIR}/lib/cmake 
            -DSeetaFaceDetector_DIR=${SeetaFace2_DIR}/lib/cmake 
            -DSeetaFaceLandmarker_DIR=${SeetaFace2_DIR}/lib/cmake 
            -DSeetaFaceRecognizer_DIR=${SeetaFace2_DIR}/lib/cmake 
            -DSeetaFaceTracker_DIR=${SeetaFace2_DIR}/lib/cmake 
            -DSeetaQualityAssessor_DIR=${SeetaFace2_DIR}/lib/cmake "
fi
echo "PARA:$PARA"

if [ ! -d build_android ]; then
    mkdir -p build_android
fi
cd build_android

cmake .. -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_VERBOSE_MAKEFILE=TRUE \
    -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_ARM_NEON=ON \
    -DBUILD_PERFORMANCE=ON \
    -DANDROID_PLATFORM=android-24 ${PARA}

cmake --build . --config Release -- -j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`

cmake --build . --config Release --target install/strip
#-- -j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`
cmake --build . --config Release --target APK

cd ..
