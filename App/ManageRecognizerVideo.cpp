/**
  @author: Kang Lin<kl222@126.com>
  */

#include "ManageRecognizerVideo.h"
#include "ui_ManageRecognizerVideo.h"
#include "RecognizerThread.h"

#include <QDebug>

CManageRecognizerVideo::CManageRecognizerVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CManageRecognizerVideo)
{
    ui->setupUi(this);

    m_pFrmRecognizerVideo = new CFrmRecognizerVideo(this); //delete it when delete this
    if(!m_pFrmRecognizerVideo)
        throw std::runtime_error("alloc memery fail");
    bool check = connect(this, SIGNAL(sigCaptureFrame(const QImage&)),
                    m_pFrmRecognizerVideo, SLOT(slotDisplay(const QImage&)));
    Q_ASSERT(check);
    check = connect(m_pFrmRecognizerVideo,
        SIGNAL(sigRecognize(const QImage&, const QVector<CTracker::strFace> &)),
        this,
       SIGNAL(sigRecognize(const QImage&, const QVector<CTracker::strFace> &)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigRecognized(const QMap<int, QString> &)),
        m_pFrmRecognizerVideo, SLOT(slotRecognized(const QMap<int, QString>&)));
    Q_ASSERT(check);

    m_pThread = new CRecognizerThread(this);
    m_pThread->start();
}

CManageRecognizerVideo::~CManageRecognizerVideo()
{
    qDebug() << "CManageRecognizerVideo::~CManageRecognizerVideo()";
    m_pThread->exit();
    delete ui;
}

void CManageRecognizerVideo::slotRecognized(const QImage &image)
{
    Q_UNUSED(image)
    m_pFrmRecognizerVideo->hide();
}

void CManageRecognizerVideo::slotRecognizerFinish()
{
    m_pFrmRecognizerVideo->show();
    m_pFrmRecognizerVideo->resize(this->size());
}

void CManageRecognizerVideo::resizeEvent(QResizeEvent *event)
{
    if(!m_pFrmRecognizerVideo->isHidden())
        m_pFrmRecognizerVideo->resize(event->size());
}
