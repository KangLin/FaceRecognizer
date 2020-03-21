project(PlugFacedetection)

find_package(facedetection)
message("== Plugs libfacedetection:${facedetection_FOUND}")
if(NOT facedetection_FOUND)
    return()
endif()

set(LIBS facedetection)

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

add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}
    VERSION ${BUILD_VERSION})
target_link_libraries(${PROJECT_NAME} PUBLIC FaceRecognizer RabbitCommon ${QT_LIBRARIES} ${LIBS})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_SOURCE_DIR}/Src
    ${CMAKE_BINARY_DIR}
    ${INCLUDE_DIRS})

if(BUILD_VERSION)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
endif()

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    COMPONENT Runtime
    LIBRARY DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    COMPONENT Runtime
    #ARCHIVE DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    )
