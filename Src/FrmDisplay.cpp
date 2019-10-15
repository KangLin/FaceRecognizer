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

void CFrmDisplay::slotDisplay(const QImage &frame)
{
    m_Image = frame;
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(this->isHidden())
        return;
    QPainter painter(this);
    painter.drawImage(this->rect(), m_Image);
}
