#include "FrmDisplay.h"
#include "ui_FrmDisplay.h"
#include <QPainter>

CFrmDisplay::CFrmDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmDisplay)
{
    ui->setupUi(this);
    m_Rotation = 0;
}

CFrmDisplay::~CFrmDisplay()
{
    delete ui;
}

void CFrmDisplay::slotDisplay(const QVideoFrame &frame)
{
    m_VideoFrame = frame;
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    if(this->isHidden())
            return;
        QPainter painter(this);
    
        if(!m_VideoFrame.isValid())
            return;
        if(!m_VideoFrame.map(QAbstractVideoBuffer::ReadOnly))
            return;
        do{
            QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(
                        m_VideoFrame.pixelFormat());
            if(QImage::Format_Invalid == f)
                break;
            //LOG_MODEL_DEBUG("CFrmPlayer", "m_VideoFrame.bytesPerLine():%d", m_VideoFrame.bytesPerLine());
            QImage image(m_VideoFrame.bits(),
                         m_VideoFrame.width(),
                         m_VideoFrame.height(),
                         m_VideoFrame.width() << 2,
                         f);
            if(m_Rotation)
                image = image.transformed(QTransform().rotate(m_Rotation));
            painter.drawImage(this->rect(), image);
        }while(0);
        m_VideoFrame.unmap();
}

int CFrmDisplay::SetCameraAngle(int rotation)
{
    m_Rotation = rotation;
    return 0;
}
