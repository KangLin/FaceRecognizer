/**
  @author: Kang Lin<kl222@126.com>
  */

#include "FrmBroweFile.h"
#include "ui_FrmBroweFile.h"
#include "RabbitCommonDir.h"

#include <QPainter>

CFrmBroweFile::CFrmBroweFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmBroweFile),
    m_bIsDirectory(false)
{
    ui->setupUi(this);
}

CFrmBroweFile::~CFrmBroweFile()
{
    delete ui;
}

int CFrmBroweFile::setFile(const QString &szFile)
{
    ui->leFile->setText(szFile);
    return 0;
}

QString CFrmBroweFile::getFile()
{
    return ui->leFile->text();
}

void CFrmBroweFile::on_pbBrowse_clicked()
{
    QString szFile;
    if(m_bIsDirectory)
        szFile = RabbitCommon::CDir::GetOpenDirectory(this,
                                                      tr("Open folder"),
                                                      getFile());
    else
        szFile = RabbitCommon::CDir::GetOpenFileName(this,
                                                     tr("Open file"),
                                                     getFile());
    if(szFile.isEmpty()) return;
    setFile(szFile);
}

int CFrmBroweFile::setStyleOption(const QStyleOptionViewItem *option)
{
    m_StyleOptioins = *option;
    ui->leFile->setPalette(option->palette);
    ui->leFile->setFont(option->font);
    this->setPalette(option->palette);
    return 0;
}

void CFrmBroweFile::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(0);
    pen.setColor(palette().color(QPalette::Window));
    painter.setPen(pen);
    painter.setBrush(palette().brush(QPalette::Window));
    painter.drawRect(rect());
    return;   
}

bool CFrmBroweFile::getIsDirectory()
{
    return m_bIsDirectory;
}

int CFrmBroweFile::setIsDirectory(bool d)
{
    m_bIsDirectory = d;
    return 0;
}
