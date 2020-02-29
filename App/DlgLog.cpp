#include "DlgLog.h"
#include "ui_DlgLog.h"

#include <QFile>

CDlgLog::CDlgLog(const QString &szLogFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgLog)
{
    ui->setupUi(this);
    m_szFile = szLogFile;
    if(!m_szFile.isEmpty())
    {
        QString szLog;
        QFile f(m_szFile);
        if(!f.open(QFile::ReadOnly))
            return;
        szLog = f.readAll();
        f.close();
        ui->textEdit->append(szLog);
    }
}

CDlgLog::~CDlgLog()
{
    delete ui;
}

void CDlgLog::on_pbOk_clicked()
{
    this->accept();
}

void CDlgLog::on_pbClean_clicked()
{
    ui->textEdit->clear();
    QFile::remove(m_szFile);
}
