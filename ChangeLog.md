- v0.0.5
  - Add set parameters in main windows dock
  - Modify Detect interface
  - Delete parameter interface
  - Use qt plugs manage ai libraries
  - Add libfacedection 
  - Add ncnn face
  - FIX: divide by 0 crash. Affects all previous releases.
    See RabbitCommon: 7ed300a206d8a36c30003b6c19ad181682d0ff5f
  - FFmpeg: update API avpicture_* to av_image_*
  - Support Qt6
  - Use [RabbitCommon v2.2.0](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.0)
  - Copy ChangeLog etc files to ${CMAKE_INSTALL_DOCDIR}
  
- v0.0.4
  - Add opencv face
    - Add opencv dnn face detect
    - Add opencv face face detect
    - Add opencv facde landmark
  - Modify face interface
    - Modify CLandmarker class
    - Modify CRecognizer class
    - Modify CFactoryFace class
  - Automatically generate android binary(APK) distributions under CI
  - UI
    - Add log dialog
    - Add select AI libraries menu
- v0.0.3
  - Add database
  - Add register from image
  - Add recognizer from image
  - Add register from video
  - Add recognizer from video
  - Use default qt in ubuntu to ci
  - Automatically generate binary distributions under CI

- v0.0.2
  - Register and recognizer face from video file
  - Add format convert
  - Add application icon

- v0.0.1
  - Face recognizer
  - Face register
  - Display carmera
