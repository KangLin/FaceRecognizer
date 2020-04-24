#!/bin/bash

function help()
{
    echo "Please set enviroment value ANDROID_NDK"
    echo "$0 QT_ROOT ThirdLibs_DIR RabbitCommon_DIR ENABLE_DOWNLOAD"
    exit -1
}

if [ -n "$1" ]; then
    QT_ROOT=$1
fi
if [ ! -f $QT_ROOT/bin/qmake ]; then
    help
fi

if [ -n "$2" ]; then
    export ThirdLibs_DIR=$2
fi
if [ ! -d "$ThirdLibs_DIR" ]; then
    help
fi
if [ -n "${ThirdLibs_DIR}" ]; then
    export SeetaFace_DIR=${SeetaFace_DIR}/lib/cmake
    export OpenCV_DIR=${ThirdLibs_DIR}/sdk/native/jni
    export protobuf_DIR=${ThirdLibs_DIR}/lib/cmake/protobuf
    export facedetection_DIR=${ThirdLibs_DIR}/lib/cmake/facedetection
    export dlib_DIR=${ThirdLibs_DIR}/lib/cmake/dlib
    export ncnn_DIR=${ThirdLibs_DIR}/lib/cmake/ncnn
    export FFMPEG_DIR=${SeetaFace_DIR}
    export YUV_DIR=${ThirdLibs_DIR}/lib/cmake
fi

if [ -n "$3" -a -z "$RabbitCommon_DIR" ]; then
    RabbitCommon_DIR=$3
fi

if [ -z "$RabbitCommon_DIR" ]; then
    RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
    help
fi

if [ -n "$4" ]; then
    export ENABLE_DOWNLOAD=$4
fi
if [ -z "$ENABLE_DOWNLOAD" ]; then
    export ENABLE_DOWNLOAD=OFF
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
export QT_ROOT=$QT_ROOT
export PATH=$QT_ROOT/bin:$PATH
ANDROID_ARM_NEON=OFF

if [ -z "$ANDROID_NDK" ]; then
    help
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

if [ -d "${ThirdLibs_DIR}" ]; then
    PARA="${PARA} -DYUV_DIR=${YUV_DIR}"
    PARA="${PARA} -DOPENSSL_ROOT_DIR=${ThirdLibs_DIR}"
    PARA="${PARA} -Ddlib_DIR=${ThirdLibs_DIR}/lib/cmake/dlib"
    PARA="${PARA} -Dncnn_DIR=${ThirdLibs_DIR}/lib/cmake/ncnn"
    PARA="${PARA} -Dfacedetection_DIR=${ThirdLibs_DIR}/lib/cmake/facedetection"
    PARA="${PARA} -DOpenCV_DIR=${OpenCV_DIR}"
    PARA="${PARA} -Dprotobuf_DIR=${ThirdLibs_DIR}/lib/cmake/protobuf"
    PARA="${PARA} -DFFMPEG_DIR=${FFMPEG_DIR}"
    export OPENSSL_ROOT_DIR=${ThirdLibs_DIR}
    PARA="${PARA} -DSeetaFace_DIR=${ThirdLibs_DIR}/lib/cmake
        -DSeetaNet_DIR=${ThirdLibs_DIR}/lib/cmake
        -DSeetaFaceDetector_DIR=${ThirdLibs_DIR}/lib/cmake
        -DSeetaFaceLandmarker_DIR=${ThirdLibs_DIR}/lib/cmake
        -DSeetaFaceRecognizer_DIR=${ThirdLibs_DIR}/lib/cmake 
        -DSeetaFaceTracker_DIR=${ThirdLibs_DIR}/lib/cmake 
        -DSeetaQualityAssessor_DIR=${ThirdLibs_DIR}/lib/cmake "
fi

if [ -z "$FFMPEG_DIR" ]; then
    PARA="${PARA} -DUSE_FFMPEG=OFF"
fi

if [ -z "$YUV_DIR" ]; then
    PARA="${PARA} -DUSE_YUV=OFF"
fi

if [ -z "$OpenCV_DIR" ]; then
    PARA="${PARA} -DUSE_OPENCV=OFF"
fi

if [ -z "$ANDROID_ABI" ]; then
    ANDROID_ABI="arm64-v8a"
    ANDROID_ARM_NEON=ON
fi

if [ -z "${ANDROID_PLATFORM}" ]; then
    ANDROID_PLATFORM="android-24"
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
    -DANDROID_ABI="${ANDROID_ABI}" \
    -DANDROID_ARM_NEON=${ANDROID_ARM_NEON} \
    -DBUILD_PERFORMANCE=ON \
    -DANDROID_PLATFORM=${ANDROID_PLATFORM} ${PARA}

cmake --build . --config Release -- -j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`

cmake --build . --config Release --target install/strip
#-- -j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`
cmake --build . --config Release --target APK

cd ..
