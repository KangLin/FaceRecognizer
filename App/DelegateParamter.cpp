/**
  @author: Kang Lin<kl222@126.com>
  */

#include "DelegateParamter.h"
#include "FrmBrowseFile.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QComboBox>

CDelegateParamter::CDelegateParamter(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QSize CDelegateParamter::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QSize s = option.rect.size();

    switch(index.data(ROLE_PROPERTY_TYPE).toInt())
    {
    case TYPE_DIRECTORY:
    case TYPE_FILE:
    {
        CFrmBrowseFile f;
        f.setStyleOption(&option);
        f.setFile(index.data(Qt::EditRole).toString());
        s.setWidth(qMax(f.sizeHint().width(), s.width()));
        s.setHeight(qMax(f.sizeHint().height(), s.height()));
        return s;
    }
    case TYPE_ENUM:
    {
        QComboBox eCombaBox;
        QString szEnum;
        szEnum = index.data(ROLE_PROPERTY_VALUE).toString();
        QStringList se;
        se = szEnum.split(";");
        foreach(auto e, se)
        {
            if(e.isEmpty())
                continue;
            QStringList szItem = e.split("=");
            eCombaBox.addItem(szItem[0], szItem[1]);
        }
        s.setWidth(qMax(eCombaBox.sizeHint().width(), s.width()));
        s.setHeight(qMax(eCombaBox.sizeHint().height(), s.height()));
        return s;
    }
    default:
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

QWidget *CDelegateParamter::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    switch(index.data(ROLE_PROPERTY_TYPE).toInt())
    {
    case TYPE_DIRECTORY:
    {
        CFrmBrowseFile *pW = new CFrmBrowseFile(parent);
        pW->setStyleOption(&option);
        pW->setFile(index.data(Qt::EditRole).toString());
        pW->setIsDirectory(true);
        pW->resize(option.rect.size());
        pW->setFocusPolicy(Qt::StrongFocus);
        pW->setAutoFillBackground(true);
        return pW;
    }
    case TYPE_FILE:
    {
        CFrmBrowseFile *pW = new CFrmBrowseFile(parent);
        pW->setStyleOption(&option);
        pW->setFile(index.data(Qt::EditRole).toString());
        pW->resize(option.rect.size());
        pW->setFocusPolicy(Qt::StrongFocus);
        pW->setAutoFillBackground(true);
        return pW;
    }
    case TYPE_ENUM:
    {
        QComboBox *pEnum = new QComboBox(parent);
        QString szEnum;
        szEnum = index.data(ROLE_PROPERTY_VALUE).toString();
        QStringList se;
        se = szEnum.split(";");
        int curIndex = 0;
        int i = 0;
        foreach(auto e, se)
        {
            if(e.isEmpty())
                continue;
            QStringList szItem = e.split("=");
            pEnum->addItem(szItem[0], szItem[1]);
            if(szItem[0] == index.data(Qt::DisplayRole).toString())
                curIndex = i;
            i++;
        }
        pEnum->setCurrentIndex(curIndex);
        return pEnum;
    }
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

}

void CDelegateParamter::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const
{
    switch(index.data(ROLE_PROPERTY_TYPE).toInt())
    {
    case TYPE_DIRECTORY:
    case TYPE_FILE:
    {
        CFrmBrowseFile *pW = dynamic_cast<CFrmBrowseFile*>(editor);
        pW->setFile(index.data(Qt::EditRole).toString());
        return;
    }
    case TYPE_ENUM:
    {
        QComboBox *pEnum = dynamic_cast<QComboBox*>(editor);
        pEnum->setCurrentText(index.data(Qt::DisplayRole).toString());
        int curIndex = 0;
        for(int i = 0; i < pEnum->count(); i++)
        {
            if(index.data(Qt::EditRole).toString()
                    == pEnum->itemData(i).toString())
                curIndex = i;
        }
        pEnum->setCurrentIndex(curIndex);
        return;
    }
    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void CDelegateParamter::setModelData(QWidget *editor,
                                     QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    switch(index.data(ROLE_PROPERTY_TYPE).toInt())
    {
    case TYPE_DIRECTORY:
    case TYPE_FILE:
    {
        CFrmBrowseFile *pW = dynamic_cast<CFrmBrowseFile*>(editor);
        model->setData(index, pW->getFile());
        return;
    }
    case TYPE_ENUM:
    {
        QComboBox *pEnum = dynamic_cast<QComboBox*>(editor);
        model->setData(index, pEnum->currentData(), Qt::EditRole);
        model->setData(index, pEnum->currentText(), Qt::DisplayRole);
        return;
    }
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
