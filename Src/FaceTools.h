#ifndef CFACETOOLS_H
#define CFACETOOLS_H

#pragma once

#include <QMetaClassInfo>
#include <QObject>
#include "facerecognizer_export.h"
#include "ParameterFaceTools.h"

class CFace;
class FACERECOGNIZER_EXPORT CFaceTools : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
public:
    explicit CFaceTools(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual int SetParameter(CParameterFaceTools *pPara);
    virtual float EvaluateQuality(const QImage &image, const QRect &face) = 0;

public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual int UpdateParameter(QString &szErr) = 0;
    CParameterFaceTools* m_pParameter;
    CFace* m_pFace;
};

#endif // CFACETOOLS_H
