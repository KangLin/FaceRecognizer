## 人脸识别
作者：康林（kl222@126.com)

[![Windows build status](https://ci.appveyor.com/api/projects/status/n50nxc7d8mm9bdpb/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/facerecognizer/branch/master)
[![Linux build Status](https://travis-ci.org/KangLin/FaceRecognizer.svg?branch=master)](https://travis-ci.org/KangLin/FaceRecognizer)

================================================================================

- [简介](#简介)
  - [支持平台](#支持平台)
  - [下载安装包](#下载安装包)
- [捐赠](#捐赠)
- [贡献](#贡献)
- [许可协议](#许可协议)
- [编译](docs/Build.md)

--------------------------------------------------------------------------------

## 简介

本项目提供一些人脸常见应用。提供应用级人脸框架，方便自定义快速开发人脸应用。
以及性能测试比较框架，方便对第三方人脸库做性能测试比较。
如有定制开发，或移植可联系本人（QQ:16614119)。加好友前请你先[捐赠](#捐赠)支持本项目，验证信息为你的捐赠额:)

- [x] 人脸识别框架
- [x] 人脸识别常见应用
  - [x] 考勤
  - [ ] 人流统计
  - [ ] 人脸特效
- [x] 测试比较工具和框架
- [ ] 人脸识别开源库测试比较
  + [x] [opencv](https://github.com/opencv/opencv), [opencv_contrib](https://github.com/opencv/opencv_contrib)
  + [ ] [dlib](https://github.com/davisking/dlib)
  + [x] [SeetaFace2](https://github.com/seetafaceengine/SeetaFace2)
  + [ ] [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)
  + [ ] [ncnn](https://github.com/Tencent/ncnn)
    - https://github.com/MirrorYuChen/ncnn_example
  + [x] 其他深度学习网络  
    - https://github.com/deepinsight/mxnet-mtcnn
    - https://github.com/SyGoing/LFFD-MNN
    - https://github.com/deepinsight/mxnet-mtcnn
    - https://github.com/imistyrain/MTCNN
    
### 支持平台

  + [x] windows (windows xp 及已后)
  + [x] linux
  + [x] android (android 21 及已后)
  + [ ] mac
  + [ ] ios
  + [x] 其它嵌入式平台，你自己移植，或者可联系本人（QQ:16614119），加好友前请你先[捐赠](#捐赠)支持本项目，验证信息为你的捐赠额:)
  
  mac 和 ios我没有设备，请有设备的同学自行编译测试。

### [下载安装包](https://github.com/KangLin/FaceRecognizer/releases/latest)

- windows
    - [FaceRecognizer-Setup-v0.0.4.exe](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.4/FaceRecognizer-Setup-v0.0.4.exe)  
  Windows安装包，支持 Windows xp 以上系统 

- android
    + [FaceRecognizer_armeabi-v7a_v0.0.4.apk](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.4/FaceRecognizer_armeabi-v7a_v0.0.4.apk)

- linux
    - [FaceRecognizer_v0.0.4.tar.gz](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.4/FaceRecognizer_v0.0.4.tar.gz)  
      AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/  
      使用:    
      1. 解压。复制FaceRecognizer_v0.0.4.tar.gz到安装目录，然后解压：

              mkdir FaceRecognizer
              cd FaceRecognizer
              cp $DOWNLOAD/FaceRecognizer_v0.0.4.tar.gz .
              tar xvfz FaceRecognizer_v0.0.4.tar.gz

      2. 安装
        
              ./install1.sh install FaceRecognizer
        
      3. 如果需要，卸载
        
              ./install1.sh remove FaceRecognizer

- ubuntu
    - [facerecognizer_0.0.4_amd64.deb](https://github.com/KangLin/FaceRecognizer/releases/download/v0.0.4/facerecognizer_0.0.4_amd64.deb)  
  deb 安装包,可用于 Ubuntu

**注意:**本项目安装包中暂时未打包模型文件，请到相关依赖项目中下载模型文件到一个目录中，然后在程序 菜单->选项->设置模型路径 中指定此目录。

## 捐赠
本软件如果对你有用，或者你喜欢它，请你捐赠，支持作者。谢谢！

![捐赠](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")

## 贡献

问题：https://github.com/KangLin/FaceRecognizer/issues  
项目位置：https://github.com/KangLin/FaceRecognizer

## 许可协议
- [本项目许可协议](License.md "License.md")  
- 第三方库许可协议：详见各第三库许可协议。当你使用本项目依赖的第三方库时，同时请遵守其许可协议。

## [编译](docs/Build.md)
