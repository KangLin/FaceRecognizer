#ifndef CTRACKER_H
#define CTRACKER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QImage>
#include <QRect>

#include "facerecognizer_export.h"
#include "ParameterDetector.h"

class CFace;
class FACERECOGNIZER_EXPORT CTracker : public QObject
{
    Q_OBJECT

public:
    explicit CTracker(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual int SetParameter(CParameterDetector *pPara);
    
    struct strFace
    {
        QRect rect;
        int pid;
    };
    
    virtual int Track(const QImage &image, QVector<strFace> &faces) = 0;

public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual int UpdateParameter(QString &szErr) = 0;
    CParameterDetector* m_pParameter;
    CFace* m_pFace;
};

Q_DECLARE_METATYPE(CTracker::strFace);
#endif // CTRACKER_H
