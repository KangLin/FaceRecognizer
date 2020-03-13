#ifndef CLANDMARKER_H_KL_2019_10_21
#define CLANDMARKER_H_KL_2019_10_21

#pragma once

#include <QObject>
#include <QImage>
#include <QRect>
#include <QString>
#include <QVector>
#include <QMetaClassInfo>

#include "facerecognizer_export.h"
#include "ParameterLandmark.h"
#include "FaceBase.h"

class CFace;
class FACERECOGNIZER_EXPORT CLandmarker : public CFaceBase
{
    Q_OBJECT
    Q_PROPERTY(int points READ getPoints WRITE setPoints)
    
public:
    CLandmarker(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual ~CLandmarker();

    virtual int Mark(const QImage &image,
                     const QRect &face,
                     QVector<QPointF> &points) = 0;
    virtual int Mark(const QImage &image,
                     const QVector<QRect> &faces,
                     QVector<QVector<QPointF> > &points);

    virtual int getPoints();
    virtual int setPoints(int nPoints);
protected:
    virtual int UpdateParameter() = 0;

    CFace* m_pFace;

private:
    int m_nPoints;
};

#endif // CLANDMARKER_H_KL_2019_10_21
