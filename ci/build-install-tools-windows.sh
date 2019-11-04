#!/bin/bash 
#下载工具  

set -ev

SOURCE_DIR="`pwd`"
echo $SOURCE_DIR
TOOLS_DIR=${SOURCE_DIR}/Tools
echo ${TOOLS_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk
    export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    export PATH=${TOOLS_DIR}/apache-ant/bin:$JAVA_HOME:$PATH
else
    exit 0
fi

if [ ! -d "${TOOLS_DIR}" ]; then
    mkdir ${TOOLS_DIR}
fi

cd ${TOOLS_DIR}

# Qt qt安装参见：https://github.com/benlau/qtci  
if [ -n "${QT_VERSION}" ]; then
    QT_DIR=C:/projects/SerialPortAssistant/Tools/Qt/${QT_VERSION}
    if [ ! -d "${QT_DIR}" ]; then
        wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-windows-x86-android-${QT_VERSION}.exe
        bash ${SOURCE_DIR}/ci/qt-installer.sh qt-opensource-windows-x86-android-${QT_VERSION}.exe ${QT_DIR}
        rm qt-opensource-windows-x86-android-${QT_VERSION}.exe
    fi
fi

#下载ANT 
wget -c -nv http://apache.fayea.com//ant/binaries/apache-ant-1.10.1-bin.tar.gz
tar xzf apache-ant-1.10.1-bin.tar.gz
rm -f apache-ant-1.10.1-bin.tar.gz
mv apache-ant-1.10.1 apache-ant

#Download android sdk  
if [ ! -d "${TOOLS_DIR}/android-sdk" ]; then
    ANDROID_STUDIO_VERSION=191.5900203
    wget -c -nv https://dl.google.com/dl/android/studio/ide-zips/3.5.1.0/android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
    unzip -q android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
    export JAVA_HOME=`pwd`/android-studio/jre
    export PATH=${JAVA_HOME}/bin:$PATH
    ANDROID_SDK_VERSION=4333796
    wget -c -nv https://dl.google.com/android/repository/sdk-tools-windows-${ANDROID_SDK_VERSION}.zip
    mkdir android-sdk
    cd android-sdk
    cp ../sdk-tools-windows-${ANDROID_SDK_VERSION}.zip .
    unzip -q sdk-tools-windows-${ANDROID_SDK_VERSION}.zip
    echo "Install sdk and ndk ......"
    cd tools
    (sleep 5 ; while true ; do sleep 1 ; printf 'y\r\n' ; done ) \
    | ./bin/sdkmanager "platform-tools" "build-tools;28.0.3" "platforms;${ANDROID_API}" "ndk-bundle"
    
    #if [ ! -d ${SOURCE_DIR}/Tools/android-ndk ]; then
    #    ln -s ${SOURCE_DIR}/Tools/android-sdk/ndk-bundle ${SOURCE_DIR}/Tools/android-ndk
    #fi
    
    #wget -c -nv https://dl.google.com/android/android-sdk_r24.4.1-windows.zip
    #unzip -q android-sdk_r24.4.1-windows.zip
    #mv android-sdk-windows android-sdk
    #rm android-sdk_r24.4.1-windows.zip
    #(sleep 5 ; while true ; do sleep 1 ; printf 'y\r\n' ; done ) \
    #| android-sdk/tools/android.bat update sdk -u -t tool,android-18,android-24,extra,platform,platform-tools,build-tools-24.0.1
fi

#下载android ndk  
#if [ ! -d "${TOOLS_DIR}/android-ndk" ]; then
#    if [ "$QT_VERSION_DIR" = "5.12" ]; then
#        wget -c -nv https://dl.google.com/android/repository/android-ndk-r19c-windows-x86_64.zip
#        unzip android-ndk-r19c-windows-x86_64.zip
#        mv android-ndk-r19c android-ndk
#        rm android-ndk-r19c-windows-x86_64.zip
#    else
#        wget -c -nv http://dl.google.com/android/ndk/android-ndk-r10e-windows-x86.exe
#        ./android-ndk-r10e-windows-x86.exe > /dev/null
#        mv android-ndk-r10e android-ndk
#        rm android-ndk-r10e-windows-x86.exe
#    fi
#fi

cd ${SOURCE_DIR}
