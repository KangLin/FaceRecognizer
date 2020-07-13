## [<img src="App/Resource/image/FaceRecognizer.ico" title="人脸识别" width="32" height="32"/>人脸识别](https://github.com/KangLin/FaceRecognizer)
作者：康林（kl222@126.com)

[![Windows build status](https://ci.appveyor.com/api/projects/status/n50nxc7d8mm9bdpb/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/facerecognizer/branch/master)
[![Linux build Status](https://travis-ci.org/KangLin/FaceRecognizer.svg?branch=master)](https://travis-ci.org/KangLin/FaceRecognizer)

--------------------------------------------------------------------------------

- [简介](#简介)
  - [支持平台](#支持平台)
  - [下载安装包](#下载安装包)
- [捐赠](#捐赠)
- [贡献](#贡献)
- [许可协议](#许可协议)
- [文档](#文档)
  - [编译文档](docs/Build.md)
  - [开发文档](docs/Develop.md)

--------------------------------------------------------------------------------

## 简介

- [项目位置](https://github.com/KangLin/FaceRecognizer)
- [项目主页](https://kanglin.github.io/FaceRecognizer/)

本项目提供一些人脸常见应用。提供应用级 AI （人脸）框架，方便自定义快速开发 AI （人脸） 应用。
以及性能测试比较框架，方便对第三方人脸库做性能测试比较。

本项目具体功能如下：

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
  + [x] [libfacedetection](https://github.com/ShiqiYu/libfacedetection)
  + [x] 其他深度学习网络  
    - https://github.com/deepinsight/mxnet-mtcnn
    - https://github.com/SyGoing/LFFD-MNN
    - https://github.com/deepinsight/mxnet-mtcnn
    - https://github.com/imistyrain/MTCNN

*图例:*

- [x] 表示当前版本已完成的功能
- [ ] 表示当前版本未完成，将来会实现的功能

### 支持平台

  + [x] windows (windows xp 及已后)
  + [x] linux
  + [x] android (android 21 及已后)
  + [ ] mac
  + [ ] ios
  + [x] 其它嵌入式平台。
  
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

**注意**: 本项目安装包中暂时未打包模型文件，请到相关依赖项目中下载模型文件到一个目录中，然后在程序 菜单->选项->设置模型路径 中指定此目录。

## 捐赠
本软件如果对你有用，或者你喜欢它，欢迎任何形式的捐赠（包括参与到项目代码贡献、意见反馈、以及捐赠资金等），支持作者。谢谢！

- 欢迎开发者参与本项目的开发，或意见反馈。
  + Issues: https://github.com/KangLin/FaceRecognizer/issues
  + 如有需要，请先提交 Issues ，再联系本人 (<QQ:16614119> ，验证信息为你提交的 Issues) ，可以为你提供必要的帮助。
- 欢迎 AI 开源库的开发者或自愿者使用本项目做为你库的测试程序或演示程序。
  向本项贡献代码、提供 AI 开源库的插件（如有需要，请联系本人 （QQ:16614119）, 可以为你提供必要的帮助）。
- 如有咨询、定制开发或移植，请你先[捐赠资金](#捐赠资金)支持本项目，
  再加联系本人(QQ:16614119)，验证信息为你的捐赠额。非诚勿扰。
- 如有需要编译程序（￥200/小时）或需要搭建整个系统（￥2000/工作日）。
  请先[付款](#捐赠资金)，再加联系本人(QQ:16614119)，验证信息为你的付款额。非诚勿扰。

**注意**: 交流时，请直奔主题，主题明确，简明扼要。

### 捐赠资金
[![捐赠](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")

- 捐赠后，请在 [Donate.md](Donate.md) 中留下你的姓名与捐赠金额
- 感谢 [Donate.md](Donate.md) 中的捐赠者对本项目的支持

## 贡献

- [问题](https://github.com/KangLin/FaceRecognizer/issues)
- [感谢所有贡献者](https://github.com/KangLin/FaceRecognizer/graphs/contributors)

## 许可协议
- [本项目许可协议](License.md "License.md")
- 第三方库许可协议：详见各第三库许可协议。当你使用本项目依赖的第三方库时，同时请遵守其许可协议。

## 文档
- [编译文档](docs/Build.md)
- [开发文档](docs/Develop.md)
