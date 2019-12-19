#ifndef CDETECTOR_H_KL_2019_10_21
#define CDETECTOR_H_KL_2019_10_21

#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QImage>
#include <QRect>

#include "facerecognizer_export.h"
#include "ParameterDetector.h"

class FACERECOGNIZER_EXPORT CDetector : public QObject
{
    Q_OBJECT
public:
    CDetector(QObject *parent = nullptr);
    virtual ~CDetector();

    virtual int SetParameter(CParameterDetector *pPara);
    virtual int Detect(const QImage &image,  QVector<QRect> &faces) = 0;

public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual int UpdateParameter(QString &szErr) = 0;
    CParameterDetector* m_pParameter;
};

#endif // CDETECTOR_H_KL_2019_10_21
