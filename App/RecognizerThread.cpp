#include "RecognizerThread.h"
#include "RecognizerVideo.h"
#include "Log.h"

CRecognizerThread::CRecognizerThread(CManageRecognizerVideo *parent) : QThread()
{
    m_pManageRecognizerVideo = dynamic_cast<CManageRecognizerVideo*>(parent);
}

CRecognizerThread::~CRecognizerThread()
{
    LOG_MODEL_DEBUG("RecognizerThread", "CRecognizerThread::~CRecognizerThread");
}

void CRecognizerThread::run()
{
    LOG_MODEL_DEBUG("RecognizerThread", "CRecognizerThread::run():%d",
                    QThread::currentThreadId());
    CRecognizerVideo recognizer;
    bool check = connect(m_pManageRecognizerVideo, SIGNAL(sigRecognize(const QImage&, const QVector<CTracker::strFace> &)),
            &recognizer, SLOT(slotRecognize(const QImage &, const QVector<CTracker::strFace> &)));
    Q_ASSERT(check);
    check = connect(&recognizer, SIGNAL(sigRecognized(const QMap<int, QString> &)),
                    m_pManageRecognizerVideo, SIGNAL(sigRecognized(const QMap<int, QString> &)));
    Q_ASSERT(check);
    
    this->exec();
}
