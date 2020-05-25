### 插件
本项目包括以下插件：

- 图像格式转换
  + [CConverFormat 接口类](../Src/ConverFormat.h)
  + 插件位置：
    - android: 应用程序目录下
    - windows、linux: 应用程序安装目录/plugs/ConverFormat
  + 插件命名：project(PlugConverXXXX)  
- 人脸识别
  + [CFace 接口类](../Src/Face.h)
  + 插件位置：
    - android: 应用程序目录下
    - windows、linux: 应用程序安装目录/plugs/Face
  + 插件命名：project(PlugFaceXXXX)  
    插件加载详见： [Src/FactoryFace.cpp 中的 CFactoryFace::FindPlugins 函数](../Src/FactoryFace.cpp)
