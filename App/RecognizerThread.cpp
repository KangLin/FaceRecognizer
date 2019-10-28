#include "RecognizerThread.h"
#include "RecognizerVideo.h"
#include <QtDebug>

CRecognizerThread::CRecognizerThread(QObject *parent) : QThread(parent)
{
    m_pManageRecognizerVideo = dynamic_cast<CManageRecognizerVideo*>(parent);
}

CRecognizerThread::~CRecognizerThread()
{

    qDebug() << "CRecognizerThread::~CRecognizerThread";
}

void CRecognizerThread::run()
{
    qDebug() << "CRecognizerThread::run():" << QThread::currentThreadId();
    CRecognizerVideo recognizer(this);
    bool check = connect(m_pManageRecognizerVideo, SIGNAL(sigRecognize(const QImage&)),
            &recognizer, SLOT(slotRecognize(const QImage &)));
    Q_ASSERT(check);
    
    this->exec();
}
