#ifndef CDETECTOR_H_KL_2019_10_21
#define CDETECTOR_H_KL_2019_10_21

#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QImage>
#include <QRect>
#include <QMetaClassInfo>

#include "facerecognizer_export.h"
#include "FaceBase.h"

class CFace;
class FACERECOGNIZER_EXPORT CDetector : public CFaceBase
{
    Q_OBJECT
    Q_CLASSINFO("Description", "Detector interface")

public:
    CDetector(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual ~CDetector();

    /**
     * @brief Detect
     * @param image:
     * @param faces
     * @return 
     */
    virtual int Detect(const QImage &image, QVector<QRect> &faces) = 0;

protected:
    CFace* m_pFace;
};

#endif // CDETECTOR_H_KL_2019_10_21
