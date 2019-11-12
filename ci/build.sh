#!/bin/bash
set -e

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi

# Download model files
echo "Download model files"
mkdir -p ${SOURCE_DIR}/model/Seeta
cd ${SOURCE_DIR}/model/Seeta
wget -c -nv https://github.com/KangLin/SeetaFace2/releases/download/model/fd_2_00.dat
wget -c -nv https://github.com/KangLin/SeetaFace2/releases/download/model/fr_2_10.dat
wget -c -nv https://github.com/KangLin/SeetaFace2/releases/download/model/pd_2_00_pts5.dat
wget -c -nv https://github.com/KangLin/SeetaFace2/releases/download/model/pd_2_00_pts81.dat

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/Tools/android-sdk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-ndk
    if [ -n "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
        export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-sdk/ndk-bundle
    fi
    if [ "$TRAVIS" = "true" ]; then
        export JAVA_HOME=${SOURCE_DIR}/Tools/android-studio/jre
        #export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
    fi
    case $BUILD_ARCH in
        arm*)
            export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
            ;;
        x86)
        export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_x86
        ;;
    esac
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
    export ANDROID_SDK=${ANDROID_SDK_ROOT}
    export ANDROID_NDK=${ANDROID_NDK_ROOT}
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    if [ "$DOWNLOAD_QT" = "TRUE" ]; then
        QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    else
        #source /opt/qt${QT_VERSION_DIR}/bin/qt${QT_VERSION_DIR}-env.sh
        export QT_ROOT=/opt/qt${QT_VERSION_DIR}
    fi
    export PATH=$QT_ROOT/bin:$PATH
    export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA="-j2"
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

echo "Build SeetaFace2 ......"
export SeetaFace2_SOURCE=${SOURCE_DIR}/../SeetaFace2
export SeetaFace2_DIR=${SeetaFace2_SOURCE}/install
git clone -b develop https://github.com/KangLin/SeetaFace2.git ${SeetaFace2_SOURCE}
cd ${SeetaFace2_SOURCE}

if [ -n "${STATIC}" ]; then
    CONFIG_PARA="${CONFIG_PARA} -DBUILD_SHARED_LIBS=${STATIC}"
fi
echo "PWD:`pwd`"
if [ "${BUILD_TARGERT}" = "android" ]; then
    cmake -G"${GENERATORS}" ${SeetaFace2_SOURCE} ${CONFIG_PARA} \
         -DCMAKE_INSTALL_PREFIX=${SeetaFace2_DIR} \
         -DCMAKE_VERBOSE=ON \
         -DCMAKE_BUILD_TYPE=Release \
         -DBUILD_EXAMPLE=OFF \
         -DANDROID_PLATFORM=${ANDROID_API} -DANDROID_ABI="${BUILD_ARCH}" \
         -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake 
else
    cmake -G"${GENERATORS}" ${SeetaFace2_SOURCE} ${CONFIG_PARA} \
         -DCMAKE_INSTALL_PREFIX=${SeetaFace2_DIR} \
         -DCMAKE_VERBOSE=ON \
         -DCMAKE_BUILD_TYPE=Release \
         -DBUILD_EXAMPLE=OFF
fi
cmake --build . --target install --config Release

cd ${SOURCE_DIR}

case ${BUILD_TARGERT} in
    windows_msvc)
        MAKE=nmake
        ;;
    windows_mingw)
        if [ "${RABBIT_BUILD_HOST}"="windows" ]; then
            MAKE="mingw32-make ${RABBIT_MAKE_JOB_PARA}"
        fi
        ;;
    *)
        MAKE="make ${RABBIT_MAKE_JOB_PARA}"
        ;;
esac

export VERSION="v0.0.2"
if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    if [ "${DOWNLOAD_QT}" != "TRUE" ]; then
        sed -i "s/export QT_VERSION_DIR=.*/export QT_VERSION_DIR=${QT_VERSION_DIR}/g" ${SOURCE_DIR}/debian/postinst
        sed -i "s/export QT_VERSION=.*/export QT_VERSION=${QT_VERSION}/g" ${SOURCE_DIR}/debian/preinst
        cat ${SOURCE_DIR}/debian/postinst
        cat ${SOURCE_DIR}/debian/preinst
    fi
    bash build_debpackage.sh ${QT_ROOT}

    sudo dpkg -i ../facerecognizer_*_amd64.deb
    echo "test ......"
    ./test/test_linux.sh

    #因为上面 dpgk 已安装好了，所以不需要设置下面的环境变量
    export LD_LIBRARY_PATH=${SeetaFace2_DIR}/bin:${SeetaFace2_DIR}/lib:${QT_ROOT}/bin:${QT_ROOT}/lib:$LD_LIBRARY_PATH
    
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
    ln -s FaceRecognizer-${VERSION}-x86_64.AppImage FaceRecognizer-x86_64.AppImage
    tar -czf FaceRecognizer_${VERSION}.tar.gz \
        FaceRecognizer-${VERSION}-x86_64.AppImage \
        FaceRecognizer-x86_64.AppImage \
        share \
        install.sh

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
        export UPLOADTOOL_BODY="Release FaceRecognizer-${VERSION}"
        #export UPLOADTOOL_PR_BODY=
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

if [ -n "$GENERATORS" ]; then
    if [ -n "${STATIC}" ]; then
        CONFIG_PARA="${CONFIG_PARA} -DBUILD_SHARED_LIBS=${STATIC}"
    fi
    if [ -n "${ANDROID_ARM_NEON}" ]; then
        CONFIG_PARA="${CONFIG_PARA} -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}"
    fi
    echo "Build FaceRecognizer ......"
    if [ "${BUILD_TARGERT}" = "android" ]; then
    	 cmake -G"${GENERATORS}" ${SOURCE_DIR} ${CONFIG_PARA} \
            -DCMAKE_INSTALL_PREFIX=`pwd`/install \
            -DCMAKE_VERBOSE=ON \
            -DCMAKE_BUILD_TYPE=Release \
            -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
            -DQt5Core_DIR=${QT_ROOT}/lib/cmake/Qt5Core \
            -DQt5Gui_DIR=${QT_ROOT}/lib/cmake/Qt5Gui \
            -DQt5Widgets_DIR=${QT_ROOT}/lib/cmake/Qt5Widgets \
            -DQt5Xml_DIR=${QT_ROOT}/lib/cmake/Qt5Xml \
            -DQt5Network_DIR=${QT_ROOT}/lib/cmake/Qt5Network \
            -DQt5Multimedia_DIR=${QT_ROOT}/lib/cmake/Qt5Multimedia \
            -DQt5Sql_DIR=${QT_ROOT}/lib/cmake/Qt5Sql \
            -DQt5LinguistTools_DIR=${QT_ROOT}/lib/cmake/Qt5LinguistTools \
            -DSeetaFace_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaNet_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceDetector_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceLandmarker_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceRecognizer_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceTracker_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaQualityAssessor_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DQt5AndroidExtras_DIR=${QT_ROOT}/lib/cmake/Qt5AndroidExtras \
            -DANDROID_PLATFORM=${ANDROID_API} -DANDROID_ABI="${BUILD_ARCH}" \
            -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake
    else
	    cmake -G"${GENERATORS}" ${SOURCE_DIR} ${CONFIG_PARA} \
            -DCMAKE_INSTALL_PREFIX=`pwd`/install \
            -DCMAKE_VERBOSE=ON \
            -DCMAKE_BUILD_TYPE=Release \
            -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
            -DSeetaFace_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaNet_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceDetector_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceLandmarker_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceRecognizer_DIR=${SeetaFace2_DIR}/lib/cmake
    fi
    cmake --build . --config Release --target install -- ${RABBIT_MAKE_JOB_PARA}
    if [ "${BUILD_TARGERT}" = "android" ]; then
        cmake --build . --target APK
        APK_FILE=`find . -name "android-build-debug.apk"`
        cp ${APK_FILE} $SOURCE_DIR
        if [ "$TRAVIS_TAG" != "" -a "$BUILD_ARCH"="armeabi-v7a" -a "$QT_VERSION"="5.13.2" ]; then

            cp $SOURCE_DIR/Update/update_android.xml .
	        
            MD5=`md5sum ${APK_FILE} | awk '{print $1}'`
            echo "MD5:${MD5}"
            sed -i "s/<VERSION>.*</<VERSION>${VERSION}</g" update_android.xml
            sed -i "s/<INFO>.*</<INFO>Release Tasks-${VERSION}</g" update_android.xml
            sed -i "s/<TIME>.*</<TIME>`date`</g" update_android.xml
            sed -i "s/<ARCHITECTURE>.*</<ARCHITECTURE>${BUILD_ARCH}</g" update_android.xml
            sed -i "s/<MD5SUM>.*</<MD5SUM>${MD5}</g" update_android.xml
            sed -i "s:<URL>.*<:<URL>https\://github.com/KangLin/FaceRecognizer/releases/download/${VERSION}/android-build-debug.apk<:g" update_android.xml

            export UPLOADTOOL_BODY="Release FaceRecognizer-${VERSION}"
            #export UPLOADTOOL_PR_BODY=
            wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
            chmod u+x upload.sh
            ./upload.sh ${APK_FILE}
            ./upload.sh update_android.xml
        fi
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
