#include "FaceOpenCV.h"

#include "DetectorOpenCV.h"
#include "LandmarkerOpenCV.h"

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorOpenCV(this);
    m_pLandmarker = new CLandmarkerOpenCV(this);
}

QString CFaceOpenCV::GetName()
{
    return "OpencvFace";
}
