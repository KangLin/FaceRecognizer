#include "ManageRecognizerVideo.h"
#include "ui_ManageRecognizerVideo.h"

CManageRecognizerVideo::CManageRecognizerVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CManageRecognizerVideo)
{
    ui->setupUi(this);

    m_pFrmRecognizerVideo = new CFrmRecognizerVideo(this);
    if(!m_pFrmRecognizerVideo)
        throw std::runtime_error("alloc memery fail");
    bool check = connect(this, SIGNAL(sigCaptureFrame(const QImage&)),
                    m_pFrmRecognizerVideo, SLOT(slotDisplay(const QImage&)));
    Q_ASSERT(check);
    check = connect(m_pFrmRecognizerVideo, SIGNAL(slotRecognizer(const QImage&)),
                         this, SLOT(slotRecognizer(const QImage&)));
    Q_ASSERT(check);
    
    m_pThread = new CRecognizerThread();
    m_pThread->start();
}

CManageRecognizerVideo::~CManageRecognizerVideo()
{
    m_pThread->exit();
    delete ui;
}

void CManageRecognizerVideo::slotRecognizer(const QImage &image)
{
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
