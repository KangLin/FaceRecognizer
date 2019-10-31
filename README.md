## 人脸识别
作者：康林（kl222@126.com)

[![Windows build status](https://ci.appveyor.com/api/projects/status/n50nxc7d8mm9bdpb/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/facerecognizer/branch/master)
[![Linux build Status](https://travis-ci.org/KangLin/FaceRecognizer.svg?branch=master)](https://travis-ci.org/KangLin/FaceRecognizer)

================================================================================

- [简介](#简介)
  - [支持平台](#支持平台)
  - [下载安装包](#下载安装包)
- [编译](#编译)
  - [依赖](#依赖)
  - [CMake 配置参数](#CMake-配置参数)
  - [各平台编译](#各平台编译)
    - [linux 平台编译说明](#linux-平台编译说明)
    - [windows 平台编译说明](#windows-平台编译说明)
    - [Android 平台编译说明](#Android-平台编译说明)
- [捐赠](#捐赠)
- [贡献](#贡献)
- [许可协议](#许可协议)

--------------------------------------------------------------------------------

## 简介

本项目提供一些人脸常见应用。提供应用级人脸框架，方便自定义快速开发人脸应用。
以及性能测试比较框架，方便对第三方人脸库做性能测试比较。
如有定制开发，或移植可联系本人（QQ:16614119)。如果对你有帮助，请你[捐赠](#捐赠)支持本项目:)

- [x] 人脸识别框架
- [x] 人脸识别常见应用
  - [x] 考勤
  - [ ] 人脸特效
- [x] 测试比较工具和框架
- [ ] 人脸识别开源库测试比较
  + [ ] [opencv](https://github.com/opencv/opencv)
  + [ ] [dlib](https://github.com/davisking/dlib)
  + [x] [SeetaFace2](https://github.com/seetafaceengine/SeetaFace2)
  + [ ] [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)

### 支持平台

  + [x] windows
  + [x] linux
  + [x] android
  + [ ] mac
  + [ ] ios
  + [x] 其它嵌入式平台，可联系本人（QQ:16614119)或你自己移植:)
  
  mac 和 ios我没有设备，请有设备的同学自行编译测试。

### [下载安装包](https://github.com/KangLin/FaceRecognizer/releases/latest)

- linux
    - [FaceRecognizer_v0.0.2.tar.gz](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.2/FaceRecognizer_v0.0.2.tar.gz)  
      AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/  
      使用:    
      1. 解压。复制FaceRecognizer_v0.0.2.tar.gz到安装目录，然后解压：

                mkdir FaceRecognizer
                cd FaceRecognizer
                cp $DOWNLOAD/FaceRecognizer_v0.0.2.tar.gz .
                tar xvfz FaceRecognizer_v0.0.2.tar.gz

      2. 安装
        
                ./install.sh install
        
      3. 如果需要，卸载
        
                ./install.sh remove

- ubuntu
    - [FaceRecognizer_0.0.2_amd64.deb](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.2/FaceRecognizer_0.0.2_amd64.deb)  
  deb 安装包,可用于 Ubuntu
  
- windows
    - [FaceRecognizer-Setup-v0.0.2.exe](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.2/FaceRecognizer-Setup-v0.0.2.exe)  
  Windows安装包，支持 Windows xp 以上系统 

- android
    + [android-build-debug.apk](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.2/android-build-debug.apk)

**注意:**本项目安装包中暂时未打包模型文件，请到相关依赖项目中下载模型文件到一个目录中，然后在程序 菜单->选项->设置模型路径 中指定此目录。

## 编译
### 下载源码

    git clone --recursive https://github.com/KangLin/FaceRecognizer.git

或者：

    git clone https://github.com/KangLin/FaceRecognizer.git
    git submodule update --init --recursive

### 依赖
+ 编译工具
  + [Qt](http://qt.io/)
  + 编译器
    - For linux or android
        + GNU Make 工具
        + GCC 或者 Clang 编译器
    - For windows
        + [MSVC](http://msdn.microsoft.com/zh-cn/vstudio)
        + MinGW
  + [CMake](http://www.cmake.org/)
+ 依赖库
  - [必选] Rabbit 公共库: https://github.com/KangLin/RabbitCommon
  - [可选] SeetaFace2: https://github.com/seetafaceengine/SeetaFace2
  - [可选] libyuv: https://github.com/KangLin/libyuv
  - [可选] ffmpeg: https://ffmpeg.org/
  - [可选] opencv: https://opencv.org/

### CMake 配置参数
  - [必选] Qt5_DIR: qt 安装位置
  - [必选] RabbitCommon_DIR: RabbitCommon 源码位置
  - [可选] HAVE_PERFORMANCE: 比较测试框架，默认 OFF
  - [可选] CMAKE_INSTALL_PREFIX: 安装前缀
  - [可选] SeetaFace_DIR: SeetaFace 库安装位置
  - [可选] YUV_DIR: libyuv 库安装位置
  - [可选] OpenCV_DIR: OpenCV 库安装位置

### 各平台编译
#### linux 平台编译说明
  - 编译

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
                 -DCMAKE_BUILD_TYPE=Release \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 [其它可选 CMake 配置参数]
        cmake --build . --config Release 

  - 安装

        cmake --build . --config Release --target install 

  - 运行例子
    + 把生成库的目录加入到变量 LD_LIBRARY_PATH 中
 
            export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/bin

    + 拷贝模型文件到程序安装目录下的 model 目录下，也可以在程序运行后指定。
      - SeetaFace 模型文件。模型文件位置详见：https://github.com/seetafaceengine/SeetaFace2

                cd FaceRecognizer
                cd build
                mkdir model
                cp fd_2_00.dat pd_2_00_pts5.dat pd_2_00_pts81.dat .

    + 执行 bin 目录下的程序

            cd FaceRecognizer
            cd build
            cd bin
            ./FaceRecognizer


#### windows 平台编译说明
  - 使用 cmake-gui.exe 工具编译。打开 cmake-gui.exe 配置
  - 命令行编译
    + 把 cmake 命令所在目录加入到环境变量 PATH 中
    + 从开始菜单打开 “VS2015开发人员命令提示”，进入命令行

      - 编译

            cd FaceRecognizer
            mkdir build
            cd build
            cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
                 -DCMAKE_BUILD_TYPE=Release \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 [其它可选 CMake 配置参数]
            cmake --build . --config Release

      - 安装

            cmake --build . --config Release --target install

      - 运行例子
        + 拷贝模型文件到程序安装目录下的 model 目录下，也可以在程序运行后指定。
          - SeetaFace 模型文件。模型文件位置详见：https://github.com/seetafaceengine/SeetaFace2
  
                  cd FaceRecognizer
                  cd build
                  mkdir model
                  cp fd_2_00.dat pd_2_00_pts5.dat pd_2_00_pts81.dat .

        + 执行 bin 目录下的程序
          - FaceRecognizer

#### Android 平台编译说明
+ 安装 ndk 编译工具
  - 从 https://developer.android.com/ndk/downloads 下载 ndk，并安装到：/home/android-ndk
  - 设置环境变量：

        export ANDROID_NDK=/home/android-ndk
        
+ 安装 sdk 工具
  - 从 https://developer.android.google.cn/studio/releases 下载 sdk tools, 并安装到 /home/android-sdk
  - 设置环境变量：
  
        export ANDROID_SDK=/home/android-sdk

+ 编译
  - 主机是 linux

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
                 -DCMAKE_BUILD_TYPE=Release \
                 -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
                 -DANDROID_ABI="armeabi-v7a with NEON" \
                 -DANDROID_PLATFORM=android-18 \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 [其它可选 CMake 配置参数]
        cmake --build . --config Release --target install
	    cmake --build . --config Release --target APK

  - 主机是 windows

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
                 -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release \
                 -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
                 -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe \
                 -DANDROID_ABI=arm64-v8a \
                 -DANDROID_ARM_NEON=ON \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 [其它可选 CMake 配置参数]
        cmake --build . --config Release --target install
	    cmake --build . --config Release --target APK

  - CMake for android 参数说明：https://developer.android.google.cn/ndk/guides/cmake
    + ANDROID_ABI: 可取下列值：
      目标 ABI。如果未指定目标 ABI，则 CMake 默认使用 armeabi-v7a。  
      有效的目标名称为：
      - armeabi：带软件浮点运算并基于 ARMv5TE 的 CPU。
      - armeabi-v7a：带硬件 FPU 指令 (VFPv3_D16) 并基于 ARMv7 的设备。
      - armeabi-v7a with NEON：与 armeabi-v7a 相同，但启用 NEON 浮点指令。这相当于设置 -DANDROID_ABI=armeabi-v7a 和 -DANDROID_ARM_NEON=ON。
      - arm64-v8a：ARMv8 AArch64 指令集。
      - x86：IA-32 指令集。
      - x86_64 - 用于 x86-64 架构的指令集。
    + ANDROID_NDK <path> 主机上安装的 NDK 根目录的绝对路径
    + ANDROID_PLATFORM: 如需平台名称和对应 Android 系统映像的完整列表，请参阅 [Android NDK 原生 API](https://developer.android.google.cn/ndk/guides/stable_apis.html)
    + ANDROID_ARM_MODE
    + ANDROID_ARM_NEON
    + ANDROID_STL:指定 CMake 应使用的 STL。默认情况下，CMake 使用 c++_static。 

  - 安装 apk 到设备

       adb install android-build-debug.apk 

## 捐赠
本软件如果对你有用，或者你喜欢它，请你捐赠，支持作者。谢谢！

![捐赠](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")

## 贡献

问题：https://github.com/KangLin/FaceRecognizer/issues  
项目位置：https://github.com/KangLin/FaceRecognizer

## 许可协议
- [本项目许可协议](License.md "License.md")  
- 第三方库许可协议：详见各第三库许可协议。当你使用本项目依赖的第三方库时，同时请遵守其许可协议。
