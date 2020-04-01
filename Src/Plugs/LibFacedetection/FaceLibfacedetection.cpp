#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"

CFaceLibFacedetection::CFaceLibFacedetection(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorLibfacedetection(this);
}

QString CFaceLibFacedetection::GetName()
{
    return "libfacedetection";
}

int CFaceLibFacedetection::GetLevel()
{
    return 3;
}
