/**
  @brief: The form is use set paramter
  @author: Kang Lin<kl222@126.com>
  */

#ifndef FRMPARA_H
#define FRMPARA_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMetaClassInfo>

namespace Ui {
class CFrmPara;
}

/**
 * @brief The CFrmPara class
 * @see CDelegateParamter
 *      CFrmBroweFile
 */
class CFrmPara : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    explicit CFrmPara(QWidget *parent = nullptr);
    ~CFrmPara();
    
public Q_SLOTS:
    int slotUpdateParamter(QAction *pAction = nullptr);
private Q_SLOTS:
    void slotItemChanged(QStandardItem *item);
    
private:
    Ui::CFrmPara *ui;
    QStandardItemModel m_Model;
    
    QStandardItem *LoadObject(QObject* pObject, QStandardItem *pRoot = nullptr);
};

#endif // FRMPARA_H
