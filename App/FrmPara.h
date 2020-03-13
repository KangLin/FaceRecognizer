/**
  @brief: The form is use set paramter
  @author: Kang Lin<kl222@126.com>
  */

#ifndef FRMPARA_H
#define FRMPARA_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class CFrmPara;
}

class CFrmPara : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmPara(QWidget *parent = nullptr);
    ~CFrmPara();
    
    int SetParamter(QObject *pPara = nullptr);
    
private Q_SLOTS:
    void slotItemChanged(QStandardItem *item);
    
private:
    Ui::CFrmPara *ui;
    QStandardItemModel m_Model;
    
    int LoadObject(QObject* pObject);  
};

#endif // FRMPARA_H
