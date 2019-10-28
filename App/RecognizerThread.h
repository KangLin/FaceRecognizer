#ifndef CRECOGNIZERTHREAD_H
#define CRECOGNIZERTHREAD_H

#pragma once
#include <QThread>
#include "ManageRecognizerVideo.h"

class CRecognizerThread : public QThread
{
    Q_OBJECT
public:
    explicit CRecognizerThread(QObject *parent = nullptr);
    virtual ~CRecognizerThread() override;

public Q_SLOTS:
    
protected:
    virtual void run() override;
    
private:
    CManageRecognizerVideo *m_pManageRecognizerVideo;
};

#endif // CRECOGNIZERTHREAD_H
