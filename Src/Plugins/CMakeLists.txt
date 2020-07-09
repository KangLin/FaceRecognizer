cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(FaceRecognizerPlugins)

#SUBDIRLIST(listPlugs ${CMAKE_CURRENT_SOURCE_DIR})
set(listPlugs LibFacedetection OpenCV ncnn Seeta FFmpge)
foreach(sub ${listPlugs})
    add_subdirectory(${sub})
    if(TARGET Plugin${sub})
        set(PLUG_LIBS ${PLUG_LIBS} Plugin${sub})
        SET(PLUG_CONTENT "${PLUG_CONTENT}\nQ_IMPORT_PLUGIN(CFace${sub})")
    endif()
endforeach()
if(NOT BUILD_SHARED_LIBS)
    set(PLUGS_SOURCES_FILE ${CMAKE_BINARY_DIR}/PluginsSource.cpp)
    
    FILE(WRITE ${PLUGS_SOURCES_FILE} "#include <QtPlugin>\n")
    FILE(APPEND ${PLUGS_SOURCES_FILE} "#include <QObject>\n")
    FILE(APPEND ${PLUGS_SOURCES_FILE} "${PLUG_CONTENT}\n")

    set(SOURCES_FILES ${SOURCES_FILES} ${PLUGS_SOURCES_FILE} PARENT_SCOPE)
    set(LIBS ${LIBS} ${PLUG_LIBS} PARENT_SCOPE)    
endif()
