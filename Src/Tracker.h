#ifndef CTRACKER_H
#define CTRACKER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QImage>
#include <QRect>
#include <QMetaClassInfo>

#include "facerecognizer_export.h"
#include "FaceBase.h"

class CFace;
class FACERECOGNIZER_EXPORT CTracker : public CFaceBase
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    explicit CTracker(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    struct strFace
    {
        QRect rect;
        int pid;
    };
    
    virtual int Track(const QImage &image, QVector<strFace> &faces) = 0;

protected:
    CFace* m_pFace;
};

Q_DECLARE_METATYPE(CTracker::strFace);
#endif // CTRACKER_H
