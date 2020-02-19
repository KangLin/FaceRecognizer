## 编译

- [依赖](#依赖)
- [CMake 配置参数](#CMake-配置参数)
- [各平台编译](#各平台编译)
  - [linux 平台编译说明](#linux-平台编译说明)
  - [windows 平台编译说明](#windows-平台编译说明)
  - [Android 平台编译说明](#Android-平台编译说明)
  
### 依赖
+ 编译工具
  + [Qt](http://qt.io/)
  + 编译器
    - For linux or android
        + GNU Make 工具
        + GCC 或者 Clang 编译器
    - For windows
        + [MSVC](http://msdn.microsoft.com/zh-cn/vstudio)
        + [MSYS2/MinGW](http://sourceforge.net/projects/msys2/)
  + [CMake](http://www.cmake.org/)
+ 依赖库
  - [必选] 玉兔公共库: https://github.com/KangLin/RabbitCommon
  - [可选] SeetaFace2: https://github.com/seetafaceengine/SeetaFace2
  - [可选] libyuv: https://github.com/KangLin/libyuv
  - [可选] ffmpeg: https://ffmpeg.org/
  - [可选] opencv: https://opencv.org/ opencv_contrib: https://opencv.org/opencv_contrib
  - [可选] dlib: https://github.com/davisking/dlib
  - [可选] ncnn:https://github.com/Tencent/ncnn
  
### CMake 配置参数
  - [必选] Qt5_DIR: qt 安装位置(指向Qt5Config.cmake的目录，默认为 安装目录/lib/cmake/Qt5)。
                   详见：https://doc.qt.io/qt-5/cmake-get-started.html
  - [必选] RabbitCommon_DIR: RabbitCommon 源码位置
  - [可选] BUILD_PERFORMANCE: 比较测试框架，默认 OFF
  - [可选] ENABLE_DOWNLOAD: 自动下载模型，默认 OFF
  - [可选] BUILD_APP: 编译应用程序,默认 ON
  - [可选] CMAKE_INSTALL_PREFIX: 安装前缀
  - [可选] SeetaFace_DIR: SeetaFace 库安装位置(指向安装的 SeetaFaceConfig.cmake 目录,默认为 安装目录/lib/cmake )
  - [可选] YUV_DIR: libyuv 库安装位置(指向安装的 YUVConfig.cmake 目录,默认为 安装目录/lib/cmake )
  - [可选] OpenCV_DIR: OpenCV 库安装位置(指向安装的 OpenCVConfig.cmake 目录)
  - [可选] dlib_DIR: dlib 库安装位置

### 各平台编译
#### 下载源码

- 下载本项目源码

    git clone --recursive https://github.com/KangLin/FaceRecognizer.git

或者：

    git clone https://github.com/KangLin/FaceRecognizer.git
    git submodule update --init --recursive

- 下载 RabbitCommon 源码

    git clone https://github.com/KangLin/RabbitCommon.git

- 从源码编译或安装可选库

#### linux 平台编译说明
- 编译

        cd FaceRecognizer
        mkdir build
        cd build
        cmake .. -G"Unix Makefiles" \
            -DCMAKE_INSTALL_PREFIX=`pwd`/install \
            -DCMAKE_BUILD_TYPE=Release \
            -DQt5_DIR= \
            -DRabbitCommon_DIR= \
            [其它可选 CMake 配置参数]
        cmake --build . --config Release 

- 安装
  + 安装库和程序

        cmake --build . --config Release --target install    # --target install/strip stip 目标
    
  + 仅安装程序

        cmake --build . --config Release --target install-runtime

- 运行例子
  + 把生成库的目录加入到变量 LD_LIBRARY_PATH 中
 
        cd FaceRecognizer
        cd build/install
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/bin:[可选库的目录]

  + 执行安装目录 bin 目录下的程序

        cd FaceRecognizer
        cd build/install/bin
        ./FaceRecognizerApp

#### windows 平台编译说明
  - 使用 cmake-gui.exe 工具编译。打开 cmake-gui.exe 配置
  - 命令行编译
    + 把 cmake 命令所在目录加入到环境变量 PATH 中
    + 从开始菜单打开 “VS2015开发人员命令提示”，进入命令行
      - 编译

            cd FaceRecognizer
            mkdir build
            cd build
            cmake .. -DCMAKE_INSTALL_PREFIX=install ^
                 -DCMAKE_BUILD_TYPE=Release ^
                 -DQt5_DIR= ^
                 -DRabbitCommon_DIR= ^
                 [其它可选 CMake 配置参数]
            cmake --build . --config Release

      - 安装
        + 安装库和程序
        
              cmake --build . --config Release --target install
        
        + 仅安装程序
        
              cmake --build . --config Release --target install-runtime

      - 运行例子
        + 执行安装目录 bin 目录下的程序

              cd FaceRecognizer
              cd build/install/bin
              FaceRecognizerApp.exe

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
    + 编译

          cd FaceRecognizer
          mkdir build
          cd build
          cmake .. -G"Unix Makefiles" \
                   -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
                   -DCMAKE_BUILD_TYPE=MinSizeRel \
                   -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
                   -DANDROID_ABI="armeabi-v7a with NEON" \
                   -DANDROID_PLATFORM=android-24 \
                   -DQt5_DIR= \
                   -DRabbitCommon_DIR= \
                   -DYUV_DIR= \
                   [其它可选 CMake 配置参数]
          cmake --build . --config MinSizeRel

    + 安装
      - 安装库和程序

          cmake --build . --config MinSizeRel --target install/strip

      - 仅安装程序

          cmake --build . --config MinSizeRel --target install-runtime

    + 生成 APK

	      cmake --build . --config MinSizeRel --target APK

  - 主机是 windows
    + windows 控制台
      - 编译

            cd FaceRecognizer
            mkdir build
            cd build
            cmake .. -DCMAKE_INSTALL_PREFIX=%cd%\android-build ^
                     -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel ^
                     -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%/build/cmake/android.toolchain.cmake ^
                     -DCMAKE_MAKE_PROGRAM=%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe ^
                     -DANDROID_ABI=arm64-v8a ^
                     -DANDROID_ARM_NEON=ON ^
                     -DANDROID_PLATFORM=android-24 ^
                     -DQt5_DIR= ^
                     -DRabbitCommon_DIR= ^
                     -DYUV_DIR= ^
                     [其它可选 CMake 配置参数]
            cmake --build . --config MinSizeRel

      - 安装
        + 安装库和程序

              cmake --build . --config MinSizeRel --target install/strip

        + 仅安装程序

              cmake --build . --config MinSizeRel --target install-runtime

        + 生成 APK

              cmake --build . --config MinSizeRel --target APK

    + msys2 或 cygwin

          cd FaceRecognizer
          mkdir build
          cd build
          cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
                   -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel \
                   -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
                   -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe \
                   -DANDROID_ABI=arm64-v8a \
                   -DANDROID_ARM_NEON=ON \
                   -DANDROID_PLATFORM=android-24 \
                   -DQt5_DIR= \
                   -DRabbitCommon_DIR= \
                   -DYUV_DIR= \
                   [其它可选 CMake 配置参数]

      - 安装
        + 安装库和程序
    
              cmake --build . --config MinSizeRel --target install/strip
    
        + 仅安装程序
    
              cmake --build . --config MinSizeRel --target install-runtime
    
        + 生成 APK
    
              cmake --build . --config MinSizeRel --target APK

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
    + ANDROID_STL: 指定 CMake 应使用的 STL。默认情况下，CMake 使用 c++_static。 
      - c++_shared: 使用 libc++ 动态库
      - c++_static: 使用 libc++ 静态库
      - none: 没有 C++ 库支持
      - system: 用系统的 STL
  - 安装 apk 到设备

       adb install android-build-debug.apk 
