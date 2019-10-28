#include "RecognizerThread.h"
#include <QtDebug>
#include <QTimer>
#include <QTime>

CRecognizerThread::CRecognizerThread(QObject *parent) : QThread(parent)
{}

CRecognizerThread::~CRecognizerThread()
{
    qDebug() << "CRecognizerThread::~CRecognizerThread";
}

void CRecognizerThread::run()
{
  
    this->exec();
}
