#ifndef CLANDMARKER_H_KL_2019_10_21
#define CLANDMARKER_H_KL_2019_10_21

#pragma once

#include <QObject>
#include <QImage>
#include <QRect>
#include <QString>
#include <QVector>

#include "facerecognizer_export.h"
#include "ParameterLandmark.h"

class CFace;
class FACERECOGNIZER_EXPORT CLandmarker : public QObject
{
    Q_OBJECT
public:
    CLandmarker(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual ~CLandmarker();

    virtual int SetParameter(CParameterLandmark *pPara);
    virtual int Mark(const QImage &image,
                     const QRect &face,
                     QVector<QPointF> &points) = 0;
    virtual int Mark(const QImage &image,
                     const QVector<QRect> &faces,
                     QVector<QVector<QPointF> > &points);

public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual int UpdateParameter(QString &szErr) = 0;
    CParameterLandmark *m_pParameter;
    CFace* m_pFace;
};

#endif // CLANDMARKER_H_KL_2019_10_21
