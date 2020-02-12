#ifndef DETECTOROPENCV_H
#define DETECTOROPENCV_H

#include "Detector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class CDetectorOpenCV : public CDetector
{
    Q_OBJECT
public:
    explicit CDetectorOpenCV(QObject *parent = nullptr);
    virtual ~CDetectorOpenCV();

    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
private:
    virtual int UpdateParameter(QString &szErr);
    bool m_bInit;
    cv::dnn::Net m_Net;
};

#endif // DETECTOROPENCV_H
