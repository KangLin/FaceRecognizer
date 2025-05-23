- v0.0.5
  - 在主窗口中增加设置属性停泊条
  - 修改接口，增加参数属性
  - 删除参数接口
  - 用 QT 插件方式管理 AI 库
  - 增加 libfacedection 实现人脸接口
  - 增加 ncnn 实现人脸接口
  - 修复除 0 引起的崩溃。影响前面所有发行版本。此错误是 RabbitCommon 引起的。
    详见：RabbitCommon: 7ed300a206d8a36c30003b6c19ad181682d0ff5f
  - FFmpeg: 兼容 API avpicture_* to av_image_*
  - 支持 Qt6
  - 使用 [RabbitCommon v2.2.0](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.0)
  - 复制日志等文件到 ${CMAKE_INSTALL_DOCDIR}

- v0.0.4
  - 增加 opencv 实现人脸接口
    - 增加 opencv dnn 人脸检测
    - 增加 opencv face 人脸检测
    - 增加 opencv facde 特征点检测
  - 修改人脸接口
    - 修改 CLandmarker 类
    - 修改 CRecognizer 类
    - 修改 CFactoryFace 类
  - 从自动化编译中分发 android  二进制包
  - 修改界面
    - 增加日志对话框
    - 增加AI库选择菜单

- v0.0.3
  - 增加数据库
  - 增加从图像注册
  - 增加从视频注册
  - 增加从图像识别
  - 增加从视频识别
  - Ubuntu 下使用系统自带 qt 自动化编译
  - CI 下自动生成二进制分发包

- v0.0.2
  - 从视频文件中注册与识别人脸
  - 增加视频格式转换
  - 增加应用图标

- v0.0.1
  - 人脸注册
  - 人脸识别
  - 显示摄像头
