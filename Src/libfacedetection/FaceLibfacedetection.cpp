#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"

CFaceLibfacedetection::CFaceLibfacedetection(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorLibfacedetection(this);
}
