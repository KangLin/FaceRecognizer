/**
  @author: Kang Lin<kl222@126.com>
  @brief: The delegate of CFrmPara
  @see: FrmPara.h
        FrmBroweFile.h
  */

#ifndef CDELEGATEPARAMTER_H
#define CDELEGATEPARAMTER_H

#include <QStyledItemDelegate>

class CDelegateParamter : public QStyledItemDelegate
{
    Q_OBJECT
  
public:
    explicit CDelegateParamter(QObject *parent = nullptr);
    enum ROLE
    {
        ROLE_PROPERTY_TYPE = Qt::UserRole,
        ROLE_PROPERTY_NAME,
        ROLE_PROPERTY_VALUE,   
        ROLE_OBJECT
    };

    enum TYPE
    {
        TYPE_OTHER,
        TYPE_ENUM,
        TYPE_DIRECTORY,
        TYPE_FILE,
        TYPE_STRING
    };

public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // CDELEGATEPARAMTER_H
