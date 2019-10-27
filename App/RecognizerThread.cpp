#include "RecognizerThread.h"
#include <QtDebug>

CRecognizerThread::CRecognizerThread(QObject *parent) : QThread(parent)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

CRecognizerThread::~CRecognizerThread()
{
   qDebug() << "CRecognizerThread::~CRecognizerThread";
}

void CRecognizerThread::run()
{

    this->exec();
}
