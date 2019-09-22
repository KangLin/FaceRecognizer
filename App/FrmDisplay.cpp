#include "FrmDisplay.h"
#include "ui_FrmDisplay.h"
#include "Tool.h"

#include <QPainter>
#include <QDebug>

#ifdef HAVE_LIBYUV
    #include "libyuv.h"
#endif

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
    m_Image = CTool::ConverFormat(frame);
    if(m_Rotation)
        m_Image.transformed(QTransform().rotate(m_Rotation));

    update();
    return;
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    if(this->isHidden())
        return;
    QPainter painter(this);
    painter.drawImage(this->rect(), m_Image);
}
