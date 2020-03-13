/**
  @author: Kang Lin<kl222@126.com>
  */

#include "FrmPara.h"
#include "ui_FrmPara.h"
#include "Log.h"
#include "FactoryFace.h"
#include "DelegateParamter.h"
#include <QDebug>
#include <QTreeView>
#include <QMetaObject>
#include <QMetaProperty>
#include <QStandardItem>

CFrmPara::CFrmPara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmPara)
{
    ui->setupUi(this);
    ui->treeView->setModel(&m_Model);
    ui->treeView->setItemDelegateForColumn(1, new CDelegateParamter(ui->treeView));
    m_Model.setHorizontalHeaderLabels(QStringList() << tr("Property") << tr("Value"));
    bool check = connect(&m_Model, SIGNAL(itemChanged(QStandardItem *)),
                         this, SLOT(slotItemChanged(QStandardItem*)));
    Q_ASSERT(check);
    
    SetParamter();
}

CFrmPara::~CFrmPara()
{
    delete ui;
}

int CFrmPara::SetParamter(QObject *pPara)
{
    Q_UNUSED(pPara)
    LoadObject(CFactoryFace::Instance()->GetDector());
    LoadObject(CFactoryFace::Instance()->GetLandmarker());
    LoadObject(CFactoryFace::Instance()->GetRecognizer());
    LoadObject(CFactoryFace::Instance()->GetTracker());
    return 0;
}

int CFrmPara::LoadObject(QObject *pObject)
{
    int nRet = 0;
    if(!pObject) return -1;

    qDebug() << "pObject:" << pObject;
    const QMetaObject *pMO = pObject->metaObject();
    for(int info = 0; info < pMO->classInfoCount(); info++)
    {
        qDebug() << "class info:" << pMO->classInfo(info).name()
                 << pMO->classInfo(info).value();
    }
    QStandardItem *pClass = new QStandardItem(pMO->className());
    pClass->setEditable(false);
    m_Model.appendRow(pClass);
    int nCount = pMO->propertyCount();
    //qDebug() << pMO->className() << "count:" << nCount;
    for(int i = 0; i < nCount; i++)
    {
        QMetaProperty p = pMO->property(i);
        if(!p.isValid())
        {
            //qDebug() << "property isn't valid";
            continue;
        }
        QStandardItem* pItem = new QStandardItem(p.name());
        pItem->setEditable(false);
        pClass->appendRow(pItem);
        QStandardItem* pValue = new QStandardItem(p.read(pObject).toString());
        pValue->setEditable(p.isWritable());
        pValue->setData(p.name(), CDelegateParamter::ROLE_PROPERTY_NAME);
        qDebug() << p.name() << p.read(pObject).toString() << p.isWritable();
        QVariant obj;
        obj.setValue(pObject);
        pValue->setData(obj, CDelegateParamter::ROLE_OBJECT);
        pClass->setChild(pItem->index().row(), 1,  pValue);
    }
    
//    int nEnumCount = pObj->enumeratorCount();
//    for(int e = 0; e < nEnumCount; e++)
//    {
//        QMetaEnum em = pObj->enumerator(e);
//        for(int j = 0; j < em.keyCount(); j++)
//            qDebug() << "enum: " << em.name() << ";key: " << em.key(j);
//    }
    return nRet;
}

void CFrmPara::slotItemChanged(QStandardItem* item)
{
    QObject* pObject = item->data(CDelegateParamter::ROLE_OBJECT).value<QObject*>();
    if(pObject)
        pObject->setProperty(
                item->data(CDelegateParamter::ROLE_PROPERTY_NAME).toString().toStdString().c_str(),
                item->data(Qt::EditRole));
}
