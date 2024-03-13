/**
  @author: Kang Lin<kl222@126.com>
  */

#include "ManageRegisterVideo.h"
#include <stdexcept>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "App.Manage.Register.Video")

CManageRegisterVideo::CManageRegisterVideo(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Register from video"));
    
    m_RegisterVideo = QSharedPointer<CFrmRegisterVideo>(new CFrmRegisterVideo(this));
    m_RegisterImage = QSharedPointer<CFrmRegisterImage>(new CFrmRegisterImage(this));
    if(!(m_RegisterImage && m_RegisterVideo))
        throw std::runtime_error("alloce memory fail");
    
    m_RegisterVideo->move(0, 0);
    m_RegisterImage->move(0, 0);
    m_RegisterImage->hide();
    
    bool check = connect(this, SIGNAL(sigCaptureFrame(const QImage &)),
                          m_RegisterVideo.data(), SLOT(slotDisplay(const QImage &)));
    Q_ASSERT(check);
    check = connect(m_RegisterVideo.data(), SIGNAL(sigRegister(const CParameterRegisterImage &)),
                    this, SLOT(slotRigister(const CParameterRegisterImage &)));
    Q_ASSERT(check);
    check = connect(m_RegisterImage.data(), SIGNAL(sigFinish()),
                    this, SLOT(slotRegisterFinish()));
    Q_ASSERT(check);
}

void CManageRegisterVideo::resizeEvent(QResizeEvent *event)
{
    if(!m_RegisterImage->isHidden())
        m_RegisterImage->resize(event->size());
    
    if(!m_RegisterVideo->isHidden())
        m_RegisterVideo->resize(event->size());
}

void CManageRegisterVideo::slotRigister(const CParameterRegisterImage &para)
{
    m_RegisterImage->slotParameter(para);
    m_RegisterImage->show();
    m_RegisterImage->resize(this->size());
    m_RegisterVideo->hide();
}

void CManageRegisterVideo::slotRegisterFinish()
{
    m_RegisterImage->hide();
    m_RegisterVideo->show();
    m_RegisterVideo->resize(size());
}
