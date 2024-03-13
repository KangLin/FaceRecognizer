#ifndef CFACETOOLS_H
#define CFACETOOLS_H

#pragma once

#include <QMetaClassInfo>
#include <QObject>
#include "facerecognizer_export.h"
#include "FaceBase.h"

class CFace;
/*!
 * \brief The CFaceTools class
 *
 * \~
 * \ingroup API_FACE
 */
class FACERECOGNIZER_EXPORT CFaceTools : public CFaceBase
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
public:
    explicit CFaceTools(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual float EvaluateQuality(const QImage &image, const QRect &face) = 0;

protected:
    CFace* m_pFace;
};

#endif // CFACETOOLS_H
