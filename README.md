## 人脸识别
作者：康林（kl222@126.com)

## 简介

- 人脸识别框架
- 人脸识别应用（考勤）
- 人脸识别开源库测试比较
  + [opencv](https://github.com/opencv/opencv)
  + [dlib](https://github.com/davisking/dlib)
  + [SeetaFace2](https://github.com/seetafaceengine/SeetaFace2)
  + [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)

### 支持平台  

  + [x] windows
  + [x] linux
  + [x] android
  + [ ] mac
  + [ ] ios
  
  mac 和 ios我没有设备，请有设备的同学自行编译测试。

[![Windows build status](https://ci.appveyor.com/api/projects/status/n50nxc7d8mm9bdpb/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/facerecognizer/branch/master)
[![Linux build Status](https://travis-ci.org/KangLin/FaceRecognizer.svg?branch=master)](https://travis-ci.org/KangLin/FaceRecognizer)

### [下载安装包](https://github.com/KangLin/FaceRecognizer/releases/latest)

- linux
    - [FaceRecognizer_v0.0.3.tar.gz](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.3/FaceRecognizer_v0.0.3.tar.gz)  
      AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/  
      使用:    
      1. 解压。复制FaceRecognizer_v0.0.3.tar.gz到安装目录，然后解压：

                mkdir FaceRecognizer
                cd FaceRecognizer
                cp $DOWNLOAD/FaceRecognizer_v0.0.3.tar.gz .
                tar xvfz FaceRecognizer_v0.0.3.tar.gz

      2. 安装
        
                ./install.sh install
        
      3. 如果需要，卸载
        
                ./install.sh remove

- ubuntu
    - [FaceRecognizer_0.0.3_amd64.deb](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.3/FaceRecognizer_0.0.3_amd64.deb)  
  deb 安装包,可用于　Ubuntu
  
- windows
    - [FaceRecognizer-Setup-v0.0.3.exe](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.3/FaceRecognizer-Setup-v0.0.3.exe)  
  Windows安装包，支持 Windows xp 以上系统 

- android
    + [android-build-debug.apk](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.3/android-build-debug.apk)

## 编译
### 编译依赖
+ 编译工具
  + [Qt](http://qt.io/)
  + 编译器
    -  For linux
        + GNU Make 工具
        + GCC 或者 Clang 编译器
    - For windows
        + [MSVC](http://msdn.microsoft.com/zh-cn/vstudio) 或者 MinGW. 
      
  + [CMake](http://www.cmake.org/)
+ 依赖库
  - [必选] [SeetaFace2](https://github.com/seetafaceengine/SeetaFace2)
  - [必选] [RabbitCommon](https://github.com/KangLin/RabbitCommon) Rabbit 公共库

### 编译参数
  - CMAKE_INSTALL_PREFIX: 安装前缀
  - Qt5_DIR: qt 安装位置
  - RabbitCommon_DIR: RabbitCommon源码位置
  - SeetaNet_DIR： SeetaNet 库安装位置
  - SeetaFaceDetector_DIR： SeetaFaceDetector 库安装位置
  - SeetaFaceLandmarker_DIR： SeetaFaceLandmarker 库安装位置
  - SeetaFaceRecognizer_DIR： SeetaFaceRecognizer 库安装位置
   
### 各平台编译
#### linux平台编译说明
  - 编译

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
                 -DCMAKE_BUILD_TYPE=Release \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 -DSeetaFace_DIR= 
        cmake --build .  --config Release 

  - 安装

        cmake --build . --target install --config Release 

  - 运行例子
    + 把生成库的目录加入到变量 LD_LIBRARY_PATH 中
 
            export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib

    + 拷贝模型文件到程序执行目录的 model 目录下

            cd FaceRecognizer
            cd build
            cd bin
            mkdir model
            cp fd_2_00.dat pd_2_00_pts5.dat pd_2_00_pts81.dat .

    + 执行 bin 目录下的程序

            cd FaceRecognizer
            cd build
            cd bin
            ./FaceRecognizer


#### windows平台编译说明
  - 使用 cmake-gui.exe 工具编译。打开 cmake-gui.exe
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
                 -DSeetaFace_DIR= 
            cmake --build . --config Release 

      - 安装

            cmake --build . --config Release --target install

      - 运行例子
        + 拷贝模型文件到程序执行目录的 model 目录下

                cd FaceRecognizer
                cd build
                cd bin
                mkdir model
                cp fd_2_00.dat pd_2_00_pts5.dat pd_2_00_pts81.dat .

        + 执行 bin 目录下的程序
          - FaceRecognizer

#### Android平台编译说明
+ 安装 ndk 编译工具
  - 从  https://developer.android.com/ndk/downloads 下载 ndk，并安装到：/home/android-ndk
  - 设置环境变量：

        export ANDROID_NDK=/home/android-ndk

+ 编译
  - 主机是linux

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake -DANDROID_ABI="armeabi-v7a with NEON" -DANDROID_PLATFORM=android-18 \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 -DSeetaFace_DIR= 
        cmake --build . --config Release --target install
	cmake --build . --target APK

  - 主机是windows

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake  -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe -DANDROID_ABI=arm64-v8a -DANDROID_ARM_NEON=ON \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= \
                 -DSeetaFace_DIR= 
        cmake --build . --config Release --target install
	cmake --build . --target APK

  - 参数说明：https://developer.android.google.cn/ndk/guides/cmake
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
    + ANDROID_STL:指定 CMake 应使用的 STL。

  - 安装 apk 到设备

       adb install android-build-debug.apk 

### 捐赠
本软件如果对你有用，或者你喜欢它，请你捐赠，支持作者。谢谢！

![捐赠](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")

### 贡献

问题：https://github.com/KangLin/FaceRecognizer/issues  
项目位置：https://github.com/KangLin/FaceRecognizer

### [许可协议](License.md "License.md")
