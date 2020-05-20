#!/bin/bash
set -e

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi
TOOLS_DIR=${SOURCE_DIR}/Tools
ThirdLibs_DIR=${TOOLS_DIR}/ThirdLibs
cd ${SOURCE_DIR}
export RabbitCommon_DIR="${SOURCE_DIR}/RabbitCommon"
export PKG_CONFIG_PATH=${ThirdLibs_DIR}/lib/pkgconfig:${PKG_CONFIG_PATH}
if [ -f ${ThirdLibs_DIR}/change_prefix.sh ]; then
    cd ${ThirdLibs_DIR}
    ./change_prefix.sh
fi

if [ -z "${ENABLE_DOWNLOAD}" ]; then
    export ENABLE_DOWNLOAD=ON
fi

function version_gt() { test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" != "$1"; }
function version_le() { test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" == "$1"; }
function version_lt() { test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" != "$1"; }
function version_ge() { test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" == "$1"; }

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk
    #if [ -n "$APPVEYOR" ]; then
        #export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    #    if [ -z "${NDK_VERSION} " ]; then
    #        NDK_VERSION=20.0.5594570
    #    fi
    #    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-sdk/ndk-bundle
    #fi
    #if [ "$TRAVIS" = "true" ]; then
        #export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
    #fi
    export JAVA_HOME=${TOOLS_DIR}/android-studio/jre
    
    if version_ge $QT_VERSION_DIR 5.14 ; then
        export QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android
    else
        case $BUILD_ARCH in
            armeabi|armeabi-v7a)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
                if [ -z "${ANDROID_ARM_NEON}" ]; then
                    ANDROID_ARM_NEON=OFF
                fi
                ;;
            "armeabi-v7a with NEON")
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
                ANDROID_ARM_NEON=ON
                ;;
            arm64-v8a)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_arm64_v8a/
                ;;
            x86)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_x86
                ;;
            x86_64)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_x86_64
                ;;
        esac
    fi
    export PATH=${TOOLS_DIR}/apache-ant/bin:$JAVA_HOME/bin:$PATH
    export ANDROID_SDK=${ANDROID_SDK_ROOT}
    export ANDROID_NDK=${ANDROID_NDK_ROOT}
    if [ -z "${BUILD_TOOS_VERSION}" ]; then
        export BUILD_TOOS_VERSION="28.0.3"
    fi
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    if [ "$DOWNLOAD_QT" = "APT" ]; then
        export QT_ROOT=/usr/lib/`uname -m`-linux-gnu/qt5
    elif [ "$DOWNLOAD_QT" = "TRUE" ]; then
        QT_DIR=${TOOLS_DIR}/Qt/${QT_VERSION}
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    else
        #source /opt/qt${QT_VERSION_DIR}/bin/qt${QT_VERSION_DIR}-env.sh
        export QT_ROOT=/opt/qt${QT_VERSION_DIR}
    fi
    export PATH=$QT_ROOT/bin:$PATH
    export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
    #export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA=""
    fi
fi

if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export PATH=/C/Qt/Tools/mingw${TOOLCHAIN_VERSION}/bin:$PATH
fi
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        export PKG_CONFIG=/c/msys64/mingw32/bin/pkg-config.exe
        RABBIT_BUILD_HOST="windows"
        if [ "$BUILD_TARGERT" = "android" ]; then
            ANDROID_NDK_HOST=windows-x86_64
            if [ ! -d $ANDROID_NDK/prebuilt/${ANDROID_NDK_HOST} ]; then
                ANDROID_NDK_HOST=windows
            fi
            CONFIG_PARA="${CONFIG_PARA} -DCMAKE_MAKE_PROGRAM=make" #${ANDROID_NDK}/prebuilt/${ANDROID_NDK_HOST}/bin/make.exe"
        fi
        ;;
    Linux* | Unix*)
    ;;
    *)
    ;;
esac

export PATH=${QT_ROOT}/bin:$PATH
echo "PATH:$PATH"
echo "PKG_CONFIG:$PKG_CONFIG"

cd ${SOURCE_DIR}

case ${BUILD_TARGERT} in
    windows_msvc)
        MAKE=nmake
        if [ -d "${ThirdLibs_DIR}" ]; then
            CONFIG_PARA="${CONFIG_PARA} -DOpenCV_DIR=${ThirdLibs_DIR}"
        fi
        ;;
    windows_mingw)
        if [ "${RABBIT_BUILD_HOST}"="windows" ]; then
            MAKE="mingw32-make ${RABBIT_MAKE_JOB_PARA}"
        fi
        if [ -d "${ThirdLibs_DIR}" ]; then
            CONFIG_PARA="${CONFIG_PARA} -DOpenCV_DIR=${ThirdLibs_DIR}"
        fi
        ;;
    unix)
        if [ -d "${ThirdLibs_DIR}" ]; then
            CONFIG_PARA="${CONFIG_PARA} -DOpenCV_DIR=${ThirdLibs_DIR}/lib/cmake/opencv4/"
        fi
        ;;
    android)
        if [ -d "${ThirdLibs_DIR}" ]; then
            CONFIG_PARA="${CONFIG_PARA} -DOpenCV_DIR=${ThirdLibs_DIR}/sdk/native/jni -DUSE_OPENCV=ON"
        fi
        ;;
    *)
        MAKE="make ${RABBIT_MAKE_JOB_PARA}"
        ;;
esac
if [ -d "${ThirdLibs_DIR}" ]; then
    CONFIG_PARA="${CONFIG_PARA} -DYUV_DIR=${ThirdLibs_DIR}/lib/cmake -DUSE_YUV=OFF"
    CONFIG_PARA="${CONFIG_PARA} -DOPENSSL_ROOT_DIR=${ThirdLibs_DIR}"
    CONFIG_PARA="${CONFIG_PARA} -Ddlib_DIR=${ThirdLibs_DIR}/lib/cmake/dlib"
    CONFIG_PARA="${CONFIG_PARA} -Dncnn_DIR=${ThirdLibs_DIR}/lib/cmake/ncnn"
    CONFIG_PARA="${CONFIG_PARA} -Dfacedetection_DIR=${ThirdLibs_DIR}/lib/cmake/facedetection"
    CONFIG_PARA="${CONFIG_PARA} -DFFMPEG_DIR=${ThirdLibs_DIR} -DUSE_FFMPEG=ON"
    CONFIG_PARA="${CONFIG_PARA} -Dprotobuf_DIR=${ThirdLibs_DIR}/lib/cmake/protobuf"
    export OPENSSL_ROOT_DIR=${ThirdLibs_DIR}
    export SeetaFace_DIR=${ThirdLibs_DIR}
fi

if [ -n "$appveyor_build_version" -a -z "$VERSION" ]; then
    export VERSION="v0.0.4"
fi
if [ -z "$VERSION" ]; then
    export VERSION="v0.0.4"
fi
export UPLOADTOOL_BODY="Release FaceRecognizer ${VERSION}.<br/> The change see [ChangeLog.md](ChangeLog.md) or [ChangeLog_zh_CN.md](ChangeLog_zh_CN.md)"
#export UPLOADTOOL_PR_BODY=
        
if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    if [ "${DOWNLOAD_QT}" != "TRUE" -a "${DOWNLOAD_QT}" != "APT" ]; then
        sed -i "s/export QT_VERSION_DIR=.*/export QT_VERSION_DIR=${QT_VERSION_DIR}/g" ${SOURCE_DIR}/debian/postinst
        sed -i "s/export QT_VERSION=.*/export QT_VERSION=${QT_VERSION}/g" ${SOURCE_DIR}/debian/preinst
        cat ${SOURCE_DIR}/debian/postinst
        cat ${SOURCE_DIR}/debian/preinst
    fi
    
    bash build_debpackage.sh ${QT_ROOT} ${ThirdLibs_DIR} ${RabbitCommon_DIR} ON

    sudo dpkg -i ../facerecognizer_*_amd64.deb
    echo "test ......"
    ./test/test_linux.sh

    #因为上面 dpgk 已安装好了，所以不需要设置下面的环境变量
    #export LD_LIBRARY_PATH=${SeetaFace_DIR}/bin:${SeetaFace_DIR}/lib:${QT_ROOT}/bin:${QT_ROOT}/lib:$LD_LIBRARY_PATH
    
    cd debian/facerecognizer/opt
    
    URL_LINUXDEPLOYQT=https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
    wget -c -nv ${URL_LINUXDEPLOYQT} -O linuxdeployqt.AppImage
    chmod a+x linuxdeployqt.AppImage

    cd FaceRecognizer
    ../linuxdeployqt.AppImage share/applications/*.desktop \
        -qmake=${QT_ROOT}/bin/qmake -appimage -no-copy-copyright-files -verbose

    # Create appimage install package
    #cp ../FaceRecognizer-${VERSION}-x86_64.AppImage .
    cp $SOURCE_DIR/Install/install.sh .
    cp $RabbitCommon_DIR/Install/install1.sh .    
    ln -s FaceRecognizer-${VERSION}-x86_64.AppImage FaceRecognizer-x86_64.AppImage
    tar -czf FaceRecognizer_${VERSION}.tar.gz \
        FaceRecognizer-${VERSION}-x86_64.AppImage \
        FaceRecognizer-x86_64.AppImage \
        share \
        install.sh \
        install1.sh

    # Create update.xml
    MD5=`md5sum $SOURCE_DIR/../facerecognizer_*_amd64.deb|awk '{print $1}'`
    echo "MD5:${MD5}"
    ./bin/FaceRecognizerApp \
        -f "`pwd`/update_linux.xml" \
        --md5 ${MD5}
    
    MD5=`md5sum FaceRecognizer_${VERSION}.tar.gz|awk '{print $1}'`
    ./FaceRecognizer-x86_64.AppImage \
        -f "`pwd`/update_linux_appimage.xml" \
        --md5 ${MD5} \
        --url "https://github.com/KangLin/FaceRecognizer/releases/download/${VERSION}/FaceRecognizer_${VERSION}.tar.gz"
    
    if [ "$TRAVIS_TAG" != "" -a "${QT_VERSION}" = "5.12.3" ]; then
        wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
        chmod u+x upload.sh
        ./upload.sh $SOURCE_DIR/../facerecognizer_*_amd64.deb
        ./upload.sh update_linux.xml update_linux_appimage.xml
        ./upload.sh FaceRecognizer_${VERSION}.tar.gz
    fi
    exit 0
fi

mkdir -p build_${BUILD_TARGERT}
cd build_${BUILD_TARGERT}

if [ -n "${STATIC}" ]; then
    CONFIG_PARA="${CONFIG_PARA} -DBUILD_SHARED_LIBS=${STATIC}"
fi
if [ -n "${ANDROID_ARM_NEON}" ]; then
    CONFIG_PARA="${CONFIG_PARA} -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}"
fi

echo "Build FaceRecognizer ......"
if [ "${BUILD_TARGERT}" = "android" ]; then
    cmake -G"${GENERATORS}" ${SOURCE_DIR} ${CONFIG_PARA} \
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
        -DENABLE_DOWNLOAD=${ENABLE_DOWNLOAD} \
        -DBUILD_APP=ON \
        -DCMAKE_VERBOSE_MAKEFILE=ON \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -DCMAKE_PREFIX_PATH=${QT_ROOT} \
        -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
        -DQt5Core_DIR=${QT_ROOT}/lib/cmake/Qt5Core \
        -DQt5Gui_DIR=${QT_ROOT}/lib/cmake/Qt5Gui \
        -DQt5Widgets_DIR=${QT_ROOT}/lib/cmake/Qt5Widgets \
        -DQt5Xml_DIR=${QT_ROOT}/lib/cmake/Qt5Xml \
        -DQt5Network_DIR=${QT_ROOT}/lib/cmake/Qt5Network \
        -DQt5Multimedia_DIR=${QT_ROOT}/lib/cmake/Qt5Multimedia \
        -DQt5Sql_DIR=${QT_ROOT}/lib/cmake/Qt5Sql \
        -DQt5LinguistTools_DIR=${QT_ROOT}/lib/cmake/Qt5LinguistTools \
        -DQt5AndroidExtras_DIR=${QT_ROOT}/lib/cmake/Qt5AndroidExtras \
        -DSeetaFace_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaNet_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceDetector_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceLandmarker_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceRecognizer_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceTracker_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaQualityAssessor_DIR=${SeetaFace_DIR}/lib/cmake \
        -DANDROID_PLATFORM=${ANDROID_API} -DANDROID_ABI="${BUILD_ARCH}" \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake
        
    cmake --build . --config MinSizeRel -- ${RABBIT_MAKE_JOB_PARA}
    cmake --build . --config MinSizeRel --target install-runtime -- ${RABBIT_MAKE_JOB_PARA}  
else
    cmake -G"${GENERATORS}" ${SOURCE_DIR} ${CONFIG_PARA} \
        -DCMAKE_INSTALL_PREFIX=`pwd`/install \
        -DCMAKE_VERBOSE_MAKEFILE=${ENABLE_DOWNLOAD} \
        -DENABLE_DOWNLOAD=ON \
        -DBUILD_APP=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
        -DSeetaFace_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaNet_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceDetector_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceLandmarker_DIR=${SeetaFace_DIR}/lib/cmake \
        -DSeetaFaceRecognizer_DIR=${SeetaFace_DIR}/lib/cmake
        
    cmake --build . --config Release -- ${RABBIT_MAKE_JOB_PARA}
    if [ "$TRAVIS_TAG" != "" ]; then
        cmake --build . --config Release --target install-runtime -- ${RABBIT_MAKE_JOB_PARA}
    else
        cmake --build . --config Release --target install -- ${RABBIT_MAKE_JOB_PARA}
    fi
fi

if [ "${BUILD_TARGERT}" = "android" ]; then
    ${QT_ROOT}/bin/androiddeployqt \
        --input `pwd`/App/android_deployment_settings.json \
        --output `pwd`/android-build \
        --android-platform ${ANDROID_API} \
        --gradle \
        --sign ${RabbitCommon_DIR}/RabbitCommon.keystore rabbitcommon \
        --storepass ${STOREPASS}
    APK_FILE=`find . -name "android-build-release-signed.apk"`
    APK_NAME=FaceRecognizer_${BUILD_ARCH}_${VERSION}.apk
    mv -f ${APK_FILE} $SOURCE_DIR/${APK_NAME}
    APK_FILE=$SOURCE_DIR/${APK_NAME}
    if [ "$TRAVIS_TAG" != "" \
         -a "$BUILD_ARCH" = "armeabi-v7a" \
         -a "$QT_VERSION" = "5.12.6" ]; then

        cp $SOURCE_DIR/Update/update_android.xml .
        
        MD5=`md5sum ${APK_FILE} | awk '{print $1}'`
        echo "MD5:${MD5}"
        sed -i "s/<VERSION>.*</<VERSION>${VERSION}</g" update_android.xml
        sed -i "s/<INFO>.*</<INFO>Release FaceRecognizer ${VERSION}</g" update_android.xml
        sed -i "s/<TIME>.*</<TIME>`date`</g" update_android.xml
        sed -i "s/<ARCHITECTURE>.*</<ARCHITECTURE>${BUILD_ARCH}</g" update_android.xml
        sed -i "s/<MD5SUM>.*</<MD5SUM>${MD5}</g" update_android.xml
        sed -i "s:<URL>.*<:<URL>https\://github.com/KangLin/FaceRecognizer/releases/download/${VERSION}/${APK_NAME}<:g" update_android.xml

        wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
        chmod u+x upload.sh
        ./upload.sh ${APK_FILE}
        ./upload.sh update_android.xml
    fi
fi

if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    if [ "${BUILD_ARCH}" = "x86" ]; then
        cp /C/OpenSSL-Win32/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win32/bin/ssleay32.dll install/bin
    elif [ "${BUILD_ARCH}" = "x64" ]; then
        cp /C/OpenSSL-Win64/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win64/bin/ssleay32.dll install/bin
    fi
    
    if [ -z "${STATIC}" ]; then
        "/C/Program Files (x86)/NSIS/makensis.exe" "Install.nsi"
        MD5=`md5sum FaceRecognizer-Setup-*.exe|awk '{print $1}'`
        echo "MD5:${MD5}"
        install/bin/FaceRecognizerApp.exe -f "`pwd`/update_windows.xml" --md5 ${MD5}
    fi
fi
