#ifndef DETECTOROPENCV_H
#define DETECTOROPENCV_H

#include "Detector.h"

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"

class CDetectorOpenCV : public CDetector
{
    Q_OBJECT
public:
    explicit CDetectorOpenCV(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
private:
    virtual int UpdateParameter();
    
    cv::Ptr<cv::CascadeClassifier> m_haar_cascade;
    bool m_bInit;
};

#endif // DETECTOROPENCV_H
