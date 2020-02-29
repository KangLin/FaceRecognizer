#include "DlgLog.h"
#include "ui_DlgLog.h"

#include <QFile>

CDlgLog::CDlgLog(const QString &szLogFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgLog)
{
    ui->setupUi(this);
    if(!szLogFile.isEmpty())
    {
        QString szLog;
        QFile f(szLogFile);
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

void CDlgLog::on_buttonBox_accepted()
{
    accept();
}
