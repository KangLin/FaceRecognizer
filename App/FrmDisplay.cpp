#include "FrmDisplay.h"
#include "ui_FrmDisplay.h"

#include <QPainter>

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

void CFrmDisplay::slotDisplay(const QImage &image)
{
    m_Image = image;
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(this->isHidden())
        return;
    QPainter painter(this);
    /*
    QImage img = m_Image.scaled(rect().size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawImage(this->rect(), img);//*/
    painter.drawImage(rect(), m_Image);
}
