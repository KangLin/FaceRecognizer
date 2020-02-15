find_package(SeetaFace)
if(SeetaFace_FOUND)
    set(LIBS_DEFINES ${LIBS_DEFINES} HAVE_SEETA_FACE)
    set(LIBS ${LIBS} ${SeetaFace_LIBRARIES})
    foreach(componet_seetaFace ${SeetaFace_LIBRARIES})
        if(ANDROID)
            INSTALL(FILES $<TARGET_FILE:${componet_seetaFace}>
                DESTINATION "libs/${ANDROID_ABI}"
                COMPONENT Runtime)
        else()
            INSTALL(FILES $<TARGET_FILE:${componet_seetaFace}>
                DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime)
        endif()
    endforeach()
    
    set(SOURCES_FILES
        ${SOURCES_FILES}
        Seeta/FaceSeeta.cpp
        Seeta/DetectorSeeta.cpp
        Seeta/TrackerSeeta.cpp
        Seeta/LandmarkerSeeta.cpp
        Seeta/RecognizerSeeta.cpp
        Seeta/FaceToolsSeeta.cpp
        )
    
    set(HEADER_FILES
        ${HEADER_FILES}
        Seeta/FaceSeeta.h
        Seeta/DetectorSeeta.h
        Seeta/TrackerSeeta.h
        Seeta/LandmarkerSeeta.h
        Seeta/RecognizerSeeta.h
        Seeta/FaceToolsSeeta.h
        )
    
    set(SOURCE_UI_FILES
        ${SOURCE_UI_FILES}
        )
    
    # Install model files
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/model/Seeta")
        if(AUTO_DOWNLOAD)
            DOWNLOAD(FILENAME "fd_2_00.dat"
                HASH e88669e5f1301ca56162de8aef1fd5d5
                URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
            DOWNLOAD(FILENAME "fr_2_10.dat"
                HASH 2d637aad8b1b7ae62154a877ec291c99
                URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
            DOWNLOAD(FILENAME "pd_2_00_pts5.dat"
                HASH 877a44aa6f07cb3064ad2828f50f261a
                URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
            DOWNLOAD(FILENAME "pd_2_00_pts81.dat"
                HASH f3f812f01121b5a80384af3c35211bdd
                URL "https://github.com/KangLin/SeetaFace2/releases/download/model/"
                DESTINATION_DIR "${CMAKE_SOURCE_DIR}/model/Seeta"
                STATUS retval RELATIVE_URL)
        else(AUTO_DOWNLOAD)
            message(AUTHOR_WARNING "Please download Seeta model files from"
                " https://github.com/KangLin/SeetaFace2/releases/tag/model"
                " to ${CMAKE_SOURCE_DIR}/model/Seeta")
        endif(AUTO_DOWNLOAD)
        
        if(EXISTS "${CMAKE_SOURCE_DIR}/model/Seeta")
            INSTALL(FILES ${CMAKE_SOURCE_DIR}/model/Seeta/fd_2_00.dat
                    ${CMAKE_SOURCE_DIR}/model/Seeta/fr_2_10.dat
                    ${CMAKE_SOURCE_DIR}/model/Seeta/pd_2_00_pts5.dat
                    ${CMAKE_SOURCE_DIR}/model/Seeta/pd_2_00_pts81.dat
                DESTINATION ${INSTALL_DATA_PREFIX}/model/Seeta
                COMPONENT Runtime)
        endif()
    endif()
endif()
