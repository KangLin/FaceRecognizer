#include "FaceOpenCVDNN.h"
#include "DetectorOpenCVDNN.h"

CFaceOpenCVDNN::CFaceOpenCVDNN(QObject *parent) : CFace(parent)
{
    m_pDetector = new CDetectorOpenCVDNN(this);
}

QString CFaceOpenCVDNN::GetName()
{
    return "OpencvDNN";
}
