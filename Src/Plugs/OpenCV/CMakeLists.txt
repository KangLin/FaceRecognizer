PROJECT(PlugOpenCV)

option(USE_OPENCV "Use opencv" ON)
if(NOT USE_OPENCV)
    return()
endif()
find_package(OpenCV)
message("== Plug OpenCV:${OpenCV_FOUND}")
if(NOT OpenCV_FOUND)
    return()
endif()

set(LIBS_DEFINES OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR})
set(LIBS ${OpenCV_LIBS})
set(INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})

set(SOURCES_FILES
    ConverFormatOpenCV.cpp
    )
set(HEADER_FILES
    ConverFormatOpenCV.h
    )

add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBS_DEFINES})
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

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Config
    RUNTIME DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
        COMPONENT Runtime
    LIBRARY DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
        COMPONENT Runtime
    ARCHIVE DESTINATION "${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}"
    )

export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
)

# Install cmake configure files
install(EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
        )
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")

foreach(opencv_component ${OpenCV_LIBS})
    if(ANDROID)
        INSTALL(FILES $<TARGET_FILE:${opencv_component}>
            DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime)
    else(ANDROID)
        INSTALL(FILES $<TARGET_FILE:${opencv_component}>
            DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime)
    endif(ANDROID)
    
    if((opencv_component STREQUAL "opencv_dnn")
            AND (OpenCV_VERSION VERSION_GREATER_EQUAL "2.4.0"))
        include(OpenCV_DNN.cmake)
    endif()
    
    if(opencv_component STREQUAL "opencv_face"
            AND OpenCV_VERSION VERSION_GREATER_EQUAL "3.4.0")
        include(OpenCV_Face.cmake)
    endif()
endforeach()
