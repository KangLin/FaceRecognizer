#include "FaceOpenCVDNN.h"
#include "FactoryFace.h"
#include "DetectorOpenCVDNN.h"

CFaceOpenCVDNN::CFaceOpenCVDNN(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorOpenCVDNN(this);
}

int CFaceOpenCVDNN::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RegisterFace("OpenCV_DNN", this, tr("OpenCV_DNN"));
    return nRet;
}

int CFaceOpenCVDNN::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("OpenCV_DNN");
    return nRet;
}
