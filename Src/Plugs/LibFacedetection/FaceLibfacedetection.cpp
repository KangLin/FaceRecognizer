#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"
#include "FactoryFace.h"

CFaceLibFacedetection::CFaceLibFacedetection(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorLibfacedetection(this);
}

int CFaceLibFacedetection::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RegisterFace("Libfacedetection", this, tr("Libfacedetection"));
    return nRet;
}

int CFaceLibFacedetection::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("Libfacedetection");
    return nRet;
}
