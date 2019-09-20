#include "FrmDisplay.h"
#include "ui_FrmDisplay.h"
#include <QPainter>
#include <QDebug>

CFrmDisplay::CFrmDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmDisplay)
{
    ui->setupUi(this);
}

CFrmDisplay::~CFrmDisplay()
{
    delete ui;
}

int CFrmDisplay::SetCameraAngle(int rotation)
{
    m_Rotation = rotation;
    return 0;
}

void CFrmDisplay::slotDisplay(const QImage &frame)
{
    m_Image = frame;
    update();
}

void CFrmDisplay::slotDisplay(const QVideoFrame& frame)
{
    qDebug() << "CFrmDisplay::slotDisplay(const QVideoFrame& frame)";
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;
    do{
        QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(
                    videoFrame.pixelFormat());
        if(QImage::Format_Invalid == f)
            break;
        QImage image(videoFrame.bits(),
                     videoFrame.width(),
                     videoFrame.height(),
                     videoFrame.width() << 2,
                     f);
        if(m_Rotation)
            image = image.transformed(QTransform().rotate(m_Rotation));

        m_Image = image;
    }while(0);
    videoFrame.unmap();
    
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    if(this->isHidden())
        return;
    QPainter painter(this);
    painter.drawImage(this->rect(), m_Image);
}
