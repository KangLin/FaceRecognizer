#ifndef DETECTOROPENCVDNN_H
#define DETECTOROPENCVDNN_H

#include "Detector.h"

#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"

class CDetectorOpenCVDNN : public CDetector
{
    Q_OBJECT
public:
    explicit CDetectorOpenCVDNN(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual ~CDetectorOpenCVDNN();

    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
private:
    virtual int UpdateParameter();
    bool m_bInit;
    cv::dnn::Net m_Net;

};

#endif // DETECTOROPENCVDNN_H
