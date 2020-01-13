export QT_ROOT=/opt/Qt5.13.2/5.13.2/android_armv7
export ANDROID_API=android-24
export BUILD_ARCH="armeabi-v7a with NEON"
export SeetaFace2_DIR=/home/SeetaFace/build_android/install/
export YUV_DIR=/home/libyuv/build_android/install/lib/cmake

cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
            -DCMAKE_VERBOSE=ON \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_PREFIX_PATH=${QT_ROOT} \
            -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
            -DQt5Core_DIR=${QT_ROOT}/lib/cmake/Qt5Core \
            -DQt5Gui_DIR=${QT_ROOT}/lib/cmake/Qt5Gui \
            -DQt5Widgets_DIR=${QT_ROOT}/lib/cmake/Qt5Widgets \
            -DQt5Xml_DIR=${QT_ROOT}/lib/cmake/Qt5Xml \
            -DQt5Network_DIR=${QT_ROOT}/lib/cmake/Qt5Network \
            -DQt5Multimedia_DIR=${QT_ROOT}/lib/cmake/Qt5Multimedia \
            -DQt5Sql_DIR=${QT_ROOT}/lib/cmake/Qt5Sql \
            -DQt5LinguistTools_DIR=${QT_ROOT}/lib/cmake/Qt5LinguistTools \
            -DSeetaFace_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaNet_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceDetector_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceLandmarker_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceRecognizer_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaFaceTracker_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DSeetaQualityAssessor_DIR=${SeetaFace2_DIR}/lib/cmake \
            -DQt5AndroidExtras_DIR=${QT_ROOT}/lib/cmake/Qt5AndroidExtras \
            -DANDROID_PLATFORM=${ANDROID_API} -DANDROID_ABI="${BUILD_ARCH}" \
            -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
            -DYUV_DIR=${YUV_DIR} -DUSE_FFMPEG=OFF -DUSE_OPENCV=OFF

