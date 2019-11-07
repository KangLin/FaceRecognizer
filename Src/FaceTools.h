#ifndef CFACETOOLS_H
#define CFACETOOLS_H

#pragma once

#include <QObject>
#include "facerecognizer_export.h"
#include "ParameterFaceTools.h"

class FACERECOGNIZER_EXPORT CFaceTools : public QObject
{
    Q_OBJECT
public:
    explicit CFaceTools(QObject *parent = nullptr);
    
    virtual int SetParameter(CParameterFaceTools *pPara);
    virtual float EvaluateQuality(const QImage &image,
                           const QRect &face,
                           const QVector<QPointF> &points) = 0;

public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual void UpdateParameter() = 0;
    CParameterFaceTools* m_pParameter;
};

#endif // CFACETOOLS_H
