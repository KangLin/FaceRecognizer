project(PlugFaceLibFacedetection)

find_package(facedetection)
message("== Plugs libfacedetection:${facedetection_FOUND}")
if(NOT facedetection_FOUND)
    return()
endif()

set(SOURCES_FILES
    FaceLibfacedetection.cpp
    DetectorLibfacedetection.cpp)
set(HEADER_FILES
    FaceLibfacedetection.h
    DetectorLibfacedetection.h)

if(ANDROID)
    INSTALL(FILES $<TARGET_FILE:facedetection>
        DESTINATION "libs/${ANDROID_ABI}")
else()
    INSTALL(FILES $<TARGET_FILE:facedetection>
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/Face)
endif()
ADD_PLUG_TARGET(NAME ${PROJECT_NAME}
    SOURCES_FILES ${SOURCES_FILES}
    HEADER_FILES ${HEADER_FILES}
    LIBS facedetection
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/Face
    INSTALL_DIR ${INSTALL_PATH}
    VERSION ${BUILD_VERSION})
