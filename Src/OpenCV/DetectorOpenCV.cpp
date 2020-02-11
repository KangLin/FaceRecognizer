#include "DetectorOpenCV.h"
// OpenCV4.0实现人脸识别: https://cloud.tencent.com/developer/article/1419671
CDetectorOpenCV::CDetectorOpenCV(QObject *parent) : CDetector(parent)
{
    
}

CDetectorOpenCV::~CDetectorOpenCV()
{
    
}

int CDetectorOpenCV::Detect(const QImage &image, QVector<QRect> &faces)
{
    int nRet = 0;
    
    return nRet;
}

int CDetectorOpenCV::UpdateParameter(QString &szErr)
{
    int nRet = 0;
    
    return nRet;
}
