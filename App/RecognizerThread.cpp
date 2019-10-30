#include "RecognizerThread.h"
#include "RecognizerVideo.h"
#include <QtDebug>

CRecognizerThread::CRecognizerThread(CManageRecognizerVideo *parent) : QThread()
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
    CRecognizerVideo recognizer;
    bool check = connect(m_pManageRecognizerVideo, SIGNAL(sigRecognize(const QImage&, const QVector<CTracker::strFace> &)),
            &recognizer, SLOT(slotRecognize(const QImage &, const QVector<CTracker::strFace> &)));
    Q_ASSERT(check);
    check = connect(&recognizer, SIGNAL(sigRecognized(const QMap<int, QString> &)),
                    m_pManageRecognizerVideo, SIGNAL(sigRecognized(const QMap<int, QString> &)));
    Q_ASSERT(check);
    
    this->exec();
}
