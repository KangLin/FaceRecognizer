/**
  @author: Kang Lin<kl222@126.com>
  */

#include "RecognizerThread.h"
#include "RecognizerVideo.h"

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(log)

CRecognizerThread::CRecognizerThread(CManageRecognizerVideo *parent) : QThread()
{
    m_pManageRecognizerVideo = dynamic_cast<CManageRecognizerVideo*>(parent);
}

CRecognizerThread::~CRecognizerThread()
{
    qDebug(log) << "CRecognizerThread::~CRecognizerThread";
}

void CRecognizerThread::run()
{
    qDebug(log) << "CRecognizerThread::run():" << QThread::currentThreadId();
    CRecognizerVideo recognizer;
    bool check = connect(m_pManageRecognizerVideo, SIGNAL(sigRecognize(const QImage&, const QVector<CTracker::strFace> &)),
            &recognizer, SLOT(slotRecognize(const QImage &, const QVector<CTracker::strFace> &)));
    Q_ASSERT(check);
    check = connect(&recognizer, SIGNAL(sigRecognized(const QMap<int, QString> &)),
                    m_pManageRecognizerVideo, SIGNAL(sigRecognized(const QMap<int, QString> &)));
    Q_ASSERT(check);
    
    this->exec();
}
