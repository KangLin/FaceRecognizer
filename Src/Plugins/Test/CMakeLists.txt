option(USE_TEST "Use test" OFF)
message(STATUS "== Use test: ${USE_TEST}")
if(NOT USE_TEST)
    return()
endif()

project(PluginTest)

set(SOURCES_FILES
    Test.cpp
    )

set(HEADER_FILES
    Test.h
    )

set(SOURCE_UI_FILES
    ${SOURCE_UI_FILES}
    )

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
