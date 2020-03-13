#ifndef CTRACKERSEETA_H
#define CTRACKERSEETA_H

#include <QObject>
#include <QSharedPointer>

#include "Tracker.h"
#include "seeta/FaceTracker.h"

class CTrackerSeeta : public CTracker
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    explicit CTrackerSeeta(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual int Track(const QImage &image, QVector<strFace> &faces);
    
private:
    virtual int UpdateParameter();

    QSharedPointer<seeta::FaceTracker> m_Tracker;
    bool m_bInit;    
};

#endif // CTRACKERSEETA_H
