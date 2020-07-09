#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"

CFaceLibFacedetection::CFaceLibFacedetection(QObject *parent) : CFace(parent)
{}

int CFaceLibFacedetection::Initialize()
{
    m_pDetector = new CDetectorLibfacedetection(this);
    return 0;
}

QString CFaceLibFacedetection::GetName()
{
    return "libfacedetection";
}

int CFaceLibFacedetection::GetLevel()
{
    return 3;
}
