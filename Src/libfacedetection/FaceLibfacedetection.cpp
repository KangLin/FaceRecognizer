#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"
#include "ParameterFactory.h"

CFaceLibfacedetection::CFaceLibfacedetection(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorLibfacedetection(this);
}
