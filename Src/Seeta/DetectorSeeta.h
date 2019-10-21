#ifndef CDETECTORSEETA_H_KL_2019_10_29
#define CDETECTORSEETA_H_KL_2019_10_29

#pragma once

#include "Detector.h"
#include "seeta/FaceDetector.h"
#include <QSharedPointer>

class CDetectorSeeta : public CDetector
{
    Q_OBJECT
public:
    CDetectorSeeta(QObject *parent = nullptr);
    virtual ~CDetectorSeeta();
    
    virtual QVector<QRect> Detect(const QImage &image);
    
private:
    virtual void UpdateParameter();
    QSharedPointer<seeta::FaceDetector> m_Dector;
};

#endif // CDETECTORSEETA_H_KL_2019_10_29
