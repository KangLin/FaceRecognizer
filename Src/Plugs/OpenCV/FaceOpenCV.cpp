#include "FaceOpenCV.h"
#include "FactoryFace.h"

#include "DetectorOpenCV.h"
#include "LandmarkerOpenCV.h"

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorOpenCV(this);
    m_pLandmarker = new CLandmarkerOpenCV(this);
}

int CFaceOpenCV::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RegisterFace("OpenCV_Face", this, tr("OpenCV_Face"));
    return nRet;
}

int CFaceOpenCV::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("OpenCV_Face");
    return nRet;
}
