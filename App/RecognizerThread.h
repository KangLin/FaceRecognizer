#ifndef CRECOGNIZERTHREAD_H
#define CRECOGNIZERTHREAD_H

#pragma once
#include <QThread>

class CRecognizerThread : public QThread
{
    Q_OBJECT
public:
    explicit CRecognizerThread(QObject *parent = nullptr);
    virtual ~CRecognizerThread() override;

protected:
    virtual void run() override;
};

#endif // CRECOGNIZERTHREAD_H
