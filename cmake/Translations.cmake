# 1. 在 CMakeLists.txt加入下列：
#   1.1 设置 TRANSLATIONS_NAME 为目标名
#   1.2. 包含本文件
#   1.3. 把 TRANSLATIONS_RESOURCE_FILES 加入到 add_executable 或 add_library 中
#    例如： 在 add_executable(${PROJECT_NAME} ${TRANSLATIONS_RESOURCE_FILES})
#   1.4. 增加目标依赖（可选，默认会自动执行）：
#    add_dependencies(${TRANSLATIONS_NAME} translations_${TRANSLATIONS_NAME})
#
#
#      例子：
#      set(TRANSLATIONS_NAME ${PROJECT_NAME})
#      include(${CMAKE_SOURCE_DIR}/cmake/Qt5CorePatches.cmake)
#      include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)
#      add_executable(${PROJECT_NAME} ${TRANSLATIONS_RESOURCE_FILES})
#      add_dependencies(${TRANSLATIONS_NAME} translations_${TRANSLATIONS_NAME})
#
# 2. 在源码 main 函数中加入, 如果是 DEBUG，需要加入宏定义 _DEBUG
#     QTranslator translator;
#     translator.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
#                   + "/" + qApp->applicationName() + "_" + QLocale::system().name() + ".qm");
#     qApp->installTranslator(&translator);

# debug 翻译资源做为资源文件嵌入程序

# android 翻译资源放在 assets 中
#
# Android:
#     assets                                       GetDirApplicationInstallRoot()  (Only read)
#        |- translations                           GetDirTranslations()
#        |        |- ${TRANSLATIONS_NAME}_zh_CN.ts
#        |        |- ${TRANSLATIONS_NAME}_zh_TW.ts
 
# 其它系统发行模式下，做为文件放在程序的安装目录 Translations 目录下
# 程序的安装目录：
#   AppRoot |
#           |- bin
#           |   |- App.exe
#           |- lib
#           |
#           |- translations
#                 |- ${TRANSLATIONS_NAME}_zh_CN.qm
#                 |- ${TRANSLATIONS_NAME}_zh_TW.qm
#
# 源码目录：
#   SourceRoot |
#              |- App
#              |   |- Resource
#              |        |-Translations
#              |             |- ${TRANSLATIONS_NAME}_zh_CN.ts
#              |             |- ${TRANSLATIONS_NAME}_zh_TW.ts
#              |- cmake
#              |   |- Translations.cmake
#              |- Src
#                  |- Resource
#                       |-Translations
#                            |- ${TRANSLATIONS_NAME}_zh_CN.ts
#                            |- ${TRANSLATIONS_NAME}_zh_TW.ts

if(NOT TRANSLATIONS_NAME)
    SET(TRANSLATIONS_NAME ${PROJECT_NAME})
endif()
message("TRANSLATIONS_NAME:${TRANSLATIONS_NAME}")
SET(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Translations/${TRANSLATIONS_NAME}_zh.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Translations/${TRANSLATIONS_NAME}_zh_CN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Translations/${TRANSLATIONS_NAME}_zh_rCN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Translations/${TRANSLATIONS_NAME}_zh_TW.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Translations/${TRANSLATIONS_NAME}_zh_rTW.ts
    )

OPTION(OPTION_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${OPTION_TRANSLATIONS}\n")
IF(OPTION_TRANSLATIONS)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        #qt5_create_translation(QM_FILES ${SOURCES_FILES} ${SOURCE_UI_FILES} ${TS_FILES}) #生成 .ts 文件与 .qm 文件，仅当没有TS文件的时候用。
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源 .qm 文件
        
        ADD_CUSTOM_TARGET(translations_${TRANSLATIONS_NAME} ALL DEPENDS ${QM_FILES})
        #add_dependencies(${TRANSLATIONS_NAME} translations_${TRANSLATIONS_NAME})
        
        set(RESOURCE_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/translations_${TRANSLATIONS_NAME}.qrc")
        if("Debug" STREQUAL CMAKE_BUILD_TYPE)
            file(WRITE "${RESOURCE_FILE_NAME}"
                "<!DOCTYPE RCC>
                <RCC version=\"1.0\">
                <qresource prefix=\"/translations\">
                ")
            foreach(qm ${QM_FILES})
                get_filename_component(qm_name ${qm} NAME)
                file(APPEND "${RESOURCE_FILE_NAME}"
                    "    <file alias=\"${qm_name}\">${qm}</file>\n")
            endforeach(qm)
            file(APPEND "${RESOURCE_FILE_NAME}"
                "  </qresource>
                </RCC>
                ")
            set(TRANSLATIONS_RESOURCE_FILES "${RESOURCE_FILE_NAME}")
        elseif(ANDROID)
            install(FILES ${QM_FILES} DESTINATION "assets/translations" COMPONENT Runtime)
        else()
            install(FILES ${QM_FILES} DESTINATION "translations" COMPONENT Runtime)
        endif()

    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_TRANSLATIONS)
