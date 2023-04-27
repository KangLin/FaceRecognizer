/**
  @author: Kang Lin<kl222@126.com>
  */

#include "FrmBrowseFile.h"
#include "ui_FrmBrowseFile.h"
#include "RabbitCommonDir.h"

#include <QPainter>

CFrmBrowseFile::CFrmBrowseFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmBrowseFile),
    m_bIsDirectory(false)
{
    ui->setupUi(this);
}

CFrmBrowseFile::~CFrmBrowseFile()
{
    delete ui;
}

int CFrmBrowseFile::setFile(const QString &szFile)
{
    ui->leFile->setText(szFile);
    return 0;
}

QString CFrmBrowseFile::getFile()
{
    return ui->leFile->text();
}

void CFrmBrowseFile::on_pbBrowse_clicked()
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

int CFrmBrowseFile::setStyleOption(const QStyleOptionViewItem *option)
{
    m_StyleOptioins = *option;
    ui->leFile->setPalette(option->palette);
    ui->leFile->setFont(option->font);
    this->setPalette(option->palette);
    return 0;
}

void CFrmBrowseFile::paintEvent(QPaintEvent *event)
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

bool CFrmBrowseFile::getIsDirectory()
{
    return m_bIsDirectory;
}

int CFrmBrowseFile::setIsDirectory(bool d)
{
    m_bIsDirectory = d;
    return 0;
}
