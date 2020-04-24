#include "FaceOpenCV.h"

#include "DetectorOpenCV.h"
#include "LandmarkerOpenCV.h"

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent)
{}

int CFaceOpenCV::Initialize()
{
    m_pDetector = new CDetectorOpenCV(this);
    m_pLandmarker = new CLandmarkerOpenCV(this);
    return 0;    
}

QString CFaceOpenCV::GetName()
{
    return "OpencvFace";
}
