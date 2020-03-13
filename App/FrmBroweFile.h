/**
  @author: Kang Lin<kl222@126.com>
  */

#ifndef FRMBROWEFILE_H
#define FRMBROWEFILE_H

#include <QWidget>
#include <QStyleOptionViewItem>

namespace Ui {
class CFrmBroweFile;
}

class CFrmBroweFile : public QWidget
{
    Q_OBJECT
  
    Q_PROPERTY(QString getFile READ getFile WRITE setFile)
    Q_PROPERTY(bool isDirectory READ getIsDirectory WRITE setIsDirectory)
    
public:
    explicit CFrmBroweFile(QWidget *parent = nullptr);
    ~CFrmBroweFile();
    
    QString getFile();
    int setFile(const QString &szFile);
    bool getIsDirectory();
    int setIsDirectory(bool d);
    
    int setStyleOption(const QStyleOptionViewItem *option);
    
private slots:
    void on_pbBrowse_clicked();
    
private:
    Ui::CFrmBroweFile *ui;

    QStyleOptionViewItem m_StyleOptioins;
    bool m_bIsDirectory;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // FRMBROWEFILE_H
