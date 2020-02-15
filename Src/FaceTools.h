#ifndef CFACETOOLS_H
#define CFACETOOLS_H

#pragma once

#include <QObject>
#include "facerecognizer_export.h"
#include "ParameterFaceTools.h"

class CFace;
class FACERECOGNIZER_EXPORT CFaceTools : public QObject
{
    Q_OBJECT
public:
    explicit CFaceTools(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual int SetParameter(CParameterFaceTools *pPara);
    virtual float EvaluateQuality(const QImage &image,
                           const QRect &face,
                           const QVector<QPointF> &points) = 0;

public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual int UpdateParameter(QString &szErr) = 0;
    CParameterFaceTools* m_pParameter;
    CFace* m_pFace;
};

#endif // CFACETOOLS_H
