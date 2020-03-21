#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"
#include "FactoryFace.h"

CFaceLibfacedetection::CFaceLibfacedetection(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorLibfacedetection(this);
}

int CFaceLibfacedetection::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RegisterFace("Libfacedetection", this, tr("Libfacedetection"));
    return nRet;
}

int CFaceLibfacedetection::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("Libfacedetection");
    return nRet;
}
