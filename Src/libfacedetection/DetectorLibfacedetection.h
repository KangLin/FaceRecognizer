#ifndef CDETECTORLIBFACEDETECTION_H
#define CDETECTORLIBFACEDETECTION_H

#pragma once
#include "Detector.h"

class CDetectorLibfacedetection : public CDetector
{
    Q_OBJECT
public:
    explicit CDetectorLibfacedetection(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
private:
    virtual int UpdateParameter();
};

#endif // CDETECTORLIBFACEDETECTION_H
