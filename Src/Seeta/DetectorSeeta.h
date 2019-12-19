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
    
    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
private:
    virtual int UpdateParameter(QString &szErr);
    QSharedPointer<seeta::FaceDetector> m_Dector;
};

#endif // CDETECTORSEETA_H_KL_2019_10_29
