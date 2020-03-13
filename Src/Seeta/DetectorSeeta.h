#ifndef CDETECTORSEETA_H_KL_2019_10_29
#define CDETECTORSEETA_H_KL_2019_10_29

#pragma once

#include "Detector.h"
#include "seeta/FaceDetector.h"
#include <QSharedPointer>
#include <QMetaClassInfo>

class CDetectorSeeta : public CDetector
{
    Q_OBJECT
    Q_PROPERTY(int minFaceSize READ getMinFaceSize WRITE setMinFaceSize)

public:
    CDetectorSeeta(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual ~CDetectorSeeta();
    
    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
    int getMinFaceSize();
    int setMinFaceSize(int size);
    
private:
    virtual int UpdateParameter(QString &szErr);
    QSharedPointer<seeta::FaceDetector> m_Dector;
    bool m_bInit;
    int m_MinFaceSize;
};

#endif // CDETECTORSEETA_H_KL_2019_10_29
