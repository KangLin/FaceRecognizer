#ifndef DETECTOROPENCV_H
#define DETECTOROPENCV_H

#include "Detector.h"
#include <opencv2/opencv.hpp>

#ifdef HAVE_OPENCV_DNN
#include <opencv2/dnn.hpp>
#endif

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
    
#ifdef HAVE_OPENCV_DNN
    virtual int UpdateParameterDNN(QString &szErr);
    virtual int DetectDNN(const QImage &image,  QVector<QRect> &faces);
    cv::dnn::Net m_Net;
#endif
};

#endif // DETECTOROPENCV_H
