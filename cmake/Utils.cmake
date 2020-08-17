include (CMakeParseArguments)

# 得到子目录
macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

function(_ADD_TARGET)
    cmake_parse_arguments(PARA "ISEXE" "NAME;OUTPUT_DIR;INSTALL_DIR;VERSION" "SOURCES_FILES;HEADER_FILES;LIBS;INCLUDE_DIRS;DEFINITIONS" ${ARGN})
    if(NOT (DEFINED PARA_NAME
            AND DEFINED PARA_INSTALL_DIR
            AND DEFINED PARA_SOURCES_FILES
            ))
        message(FATAL_ERROR "Use:
            ADD_PLUG_TARGET
                NAME name
                [ISEXE]
                SOURCES_FILES source1 [source2 ...] 
                [HEADER_FILES header1 [header2 ...]]
                [LIBS lib1 [lib2 ...]]
                [INCLUDE_DIRS [include_dir1 ...]]
                [DEFINITIONS [definition1 ...]]
                [OUTPUT_DIR output_dir]
                INSTALL_DIR install_dir
                [VERSION version]")
        return()
    endif()
    if(${PARA_ISEXE})
        add_executable(${PARA_NAME} ${PARA_SOURCES_FILES} ${PARA_HEADER_FILES})
    else()
        add_library(${PARA_NAME} ${PARA_SOURCES_FILES} ${PARA_HEADER_FILES})
    endif()

    if(DEFINED PARA_OUTPUT_DIR)
        set_target_properties(${PARA_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PARA_OUTPUT_DIR}
            ARCHIVE_OUTPUT_DIRECTORY ${PARA_OUTPUT_DIR}
            RUNTIME_OUTPUT_DIRECTORY ${PARA_OUTPUT_DIR}
            )
    endif()
    if(DEFINED PARA_VERSION)
        set_target_properties(${PARA_NAME} PROPERTIES
            VERSION ${PARA_VERSION})
    endif()
    
    if(DEFINED PARA_LIBS)
        target_link_libraries(${PARA_NAME} PUBLIC ${PARA_LIBS})
    endif()
    
    if(DEFINED PARA_DEFINITIONS)
        target_compile_definitions(${PARA_NAME} PUBLIC ${PARA_DEFINITIONS})
    endif()
    
    if(DEFINED PARA_INCLUDE_DIRS)
        target_include_directories(${PARA_NAME}
            PRIVATE ${PARA_INCLUDE_DIRS})
    endif()
    
    # Install target
    INSTALL(TARGETS ${PARA_NAME}
        EXPORT ${PARA_NAME}Config
        RUNTIME DESTINATION "${PARA_INSTALL_DIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${PARA_INSTALL_DIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${PARA_INSTALL_DIR}"
        )
    
    export(TARGETS ${PARA_NAME}
           APPEND FILE ${CMAKE_BINARY_DIR}/${PARA_NAME}Config.cmake
    )
    
    # Install cmake configure files
    install(EXPORT ${PARA_NAME}Config
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
            )

    if(DEFINED PARA_VERSION)
        write_basic_package_version_file(
            "${CMAKE_BINARY_DIR}/${PARA_NAME}ConfigVersion.cmake"
            VERSION ${PARA_VERSION}
            COMPATIBILITY AnyNewerVersion)
        install(FILES "${CMAKE_BINARY_DIR}/${PARA_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")
    endif()
endfunction()

function(ADD_PLUG_TARGET)
    cmake_parse_arguments(PARA "" "NAME;OUTPUT_DIR;INSTALL_DIR;VERSION" "SOURCES_FILES;HEADER_FILES;LIBS;INCLUDE_DIRS;DEFINITIONS" ${ARGN})
    _ADD_TARGET(NAME ${PARA_NAME}
        OUTPUT_DIR ${PARA_OUTPUT_DIR}
        INSTALL_DIR ${PARA_INSTALL_DIR}
        VERSION ${PARA_VERSION}
        SOURCES_FILES ${PARA_SOURCES_FILES}
        HEADER_FILES ${PARA_HEADER_FILES}
        LIBS FaceRecognizer RabbitCommon ${QT_LIBRARIES} ${PARA_LIBS}
        INCLUDE_DIRS ${RabbitCommon_DIR}/Src
            ${CMAKE_SOURCE_DIR}/Src
            ${CMAKE_BINARY_DIR}
            ${PARA_INCLUDE_DIRS}
        DEFINITIONS ${PARA_DEFINITIONS}
        )
endfunction()

function(INSTALL_TARGETS)
    cmake_parse_arguments(PARA "" "" "TARGETS" ${ARGN})
    if(NOT DEFINED PARA_TARGETS)
        return()
    endif()

    foreach(component ${PARA_TARGETS})
        if(ANDROID)
            INSTALL(FILES $<TARGET_FILE:${component}>
                DESTINATION "libs/${ANDROID_ABI}"
                    COMPONENT Runtime)
        elseif(WIN32)
            INSTALL(FILES $<TARGET_FILE:${component}>
                DESTINATION "${CMAKE_INSTALL_BINDIR}"
                    COMPONENT Runtime)
        else()
            INSTALL(FILES $<TARGET_FILE:${component}>
                DESTINATION "${CMAKE_INSTALL_LIBDIR}"
                    COMPONENT Runtime)
        endif()
    endforeach()
endfunction()
