#include "FaceOpenCVDNN.h"
#include "DetectorOpenCVDNN.h"

CFaceOpenCVDNN::CFaceOpenCVDNN(QObject *parent) : CFace(parent)
{}

int CFaceOpenCVDNN::Initialize()
{
    m_pDetector = new CDetectorOpenCVDNN(this);
    return 0;
}

QString CFaceOpenCVDNN::GetName()
{
    return "OpencvDNN";
}
