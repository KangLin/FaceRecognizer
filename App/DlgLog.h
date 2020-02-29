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
    void on_buttonBox_accepted();
    
private:
    Ui::CDlgLog *ui;
};

#endif // DLGLOG_H
