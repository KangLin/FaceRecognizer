set -e

PLATFORM=android
if [ -n "$1" ]; then
    PLATFORM=$1
fi

if [ "$PLATFORM" = "android" ]; then
    PARA="-DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake "
    Qt5_DIR=/opt/Qt5.12.4/5.12.4/android_armv7/lib/cmake/Qt5
fi
SEETA_DIR=/home/SeetaFace2/build_android/install/lib/cmake
if [ ! -d build_$PLATFORM ]; then
    mkdir -p build_$PLATFORM
fi
cd build_$PLATFORM

cmake .. -DCMAKE_BUILD_TYPE=Release ${PARA} \
    -DANDROID_ABI="armeabi-v7a with NEON" -DANDROID_PLATFORM=android-18 \
    -DQt5_DIR=$Qt5_DIR \
    -DQt5Core_DIR=$Qt5_DIR/../Qt5Core \
    -DQt5Gui_DIR=$Qt5_DIR/../Qt5Gui \
    -DQt5Widgets_DIR=$Qt5_DIR/../Qt5Widgets \
    -DQt5Network_DIR=$Qt5_DIR/../Qt5Network \
    -DQt5Xml_DIR=$Qt5_DIR/../Qt5Xml \
    -DQt5Multimedia_DIR=$Qt5_DIR/../Qt5Multimedia \
    -DQt5LinguistTools_DIR=$Qt5_DIR/../Qt5LinguistTools \
    -DSeetaNet_DIR=${SEETA_DIR} \
    -DSeetaFaceDetector_DIR=${SEETA_DIR} \
    -DSeetaFaceLandmarker_DIR=${SEETA_DIR} \
    -DSeetaFaceRecognizer_DIR=${SEETA_DIR}
cmake --build . --config Release #--target install
