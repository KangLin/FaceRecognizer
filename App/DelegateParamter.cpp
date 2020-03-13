/**
  @author: Kang Lin<kl222@126.com>
  */

#include "DelegateParamter.h"
#include "FrmBroweFile.h"
#include "Log.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>

CDelegateParamter::CDelegateParamter(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QSize CDelegateParamter::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QRegExp rx(".*[Pp]ath.*");
    if(!rx.exactMatch(index.data(ROLE_PROPERTY_NAME).toString())) 
        return QStyledItemDelegate::sizeHint(option, index);
    
    QSize s = option.rect.size();
    CFrmBroweFile f;
    f.setStyleOption(&option);
    f.setFile(index.data(Qt::EditRole).toString());
    s.setWidth(qMax(f.sizeHint().width(), s.width()));
    s.setHeight(qMax(f.sizeHint().height(), s.height()));
   
    return s;
}

QWidget *CDelegateParamter::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    //qDebug() << "CDelegateParamter::createEditor" << parent->metaObject()->className() << index;
    QRegExp rx(".*[Pp]ath.*");
    if(!rx.exactMatch(index.data(ROLE_PROPERTY_NAME).toString())) 
        return QStyledItemDelegate::createEditor(parent, option, index);

    CFrmBroweFile *pW = new CFrmBroweFile(parent);
    pW->setStyleOption(&option);
    pW->setFile(option.text);
    pW->setIsDirectory(true);
    pW->resize(option.rect.size());
    pW->setFocusPolicy(Qt::StrongFocus);
    pW->setAutoFillBackground(true);
    return pW;
}

void CDelegateParamter::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const
{
    //qDebug() << "CDelegateParamter::setEditorData" << editor << index;
    if(editor->inherits("CFrmBroweFile"))
    {
        CFrmBroweFile *pW = dynamic_cast<CFrmBroweFile*>(editor);
        pW->setFile(index.data(Qt::EditRole).toString());
    } else{
        QStyledItemDelegate::setEditorData(editor, index);
    }
    
    return;
}

void CDelegateParamter::setModelData(QWidget *editor,
                                     QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    //qDebug() << "CDelegateParamter::setModelData" << editor << index;
    if(editor->inherits("CFrmBroweFile"))
    {
        CFrmBroweFile *pW = dynamic_cast<CFrmBroweFile*>(editor);
        model->setData(index, pW->getFile());
    }else{
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
