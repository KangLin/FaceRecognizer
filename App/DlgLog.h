/**
  @author: Kang Lin<kl222@126.com>
  */

#ifndef DLGLOG_H
#define DLGLOG_H

#include <QDialog>

namespace Ui {
class CDlgLog;
}

class CDlgLog : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgLog(const QString &szLogFile = QString(),
                     QWidget *parent = nullptr);
    ~CDlgLog();
    
private slots:
    void on_pbOk_clicked();
    
    void on_pbClean_clicked();
    
private:
    Ui::CDlgLog *ui;
    
    QString m_szFile;
};

#endif // DLGLOG_H
