/**
  * @author: Kang Lin <kl222@126.com>
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
#include <QHeaderView>

CFrmPara::CFrmPara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmPara)
{
    ui->setupUi(this);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->setModel(&m_Model);
    ui->treeView->setItemDelegateForColumn(1, new CDelegateParamter(ui->treeView));
    //ui->treeView->setItemDelegate(new CDelegateParamter(ui->treeView));

    //slotUpdateParamter();
}

CFrmPara::~CFrmPara()
{
    delete ui;
}

int CFrmPara::slotUpdateParamter(QAction *pAction)
{
    Q_UNUSED(pAction)
    
    CFace * pFace = CFactoryFace::Instance()->GetFace();
    if(!pFace)
        return -1;
    
    bool check = m_Model.disconnect(&m_Model, SIGNAL(itemChanged(QStandardItem*)),
                                    this, SLOT(slotItemChanged(QStandardItem*)));
    //Q_ASSERT(check);
    m_Model.clear();
    
    //qDebug() << "CFrmPara::slotUpdateParamter";
    m_Model.setHorizontalHeaderLabels(QStringList() << tr("Property") << tr("Value"));

    QStandardItem* pRoot = LoadObject(pFace);
    
    CDetector* pDector = pFace->GetDector();
    if(pDector)
        LoadObject(pDector, pRoot);
    else 
        LoadObject(CFactoryFace::Instance()->GetDector());
        
    CLandmarker* pLandmarker = pFace->GetLandmarker();
    if(pLandmarker)
        LoadObject(pLandmarker, pRoot);
    else
        LoadObject(CFactoryFace::Instance()->GetLandmarker());
    
    CRecognizer* pRecgnizer = pFace->GetRecognizer();
    if(pRecgnizer)
        LoadObject(pRecgnizer, pRoot);
    else 
        LoadObject(CFactoryFace::Instance()->GetRecognizer());
    
    CTracker* pTracker = pFace->GetTracker();
    if(pTracker)
        LoadObject(pTracker, pRoot);
    else
        LoadObject(CFactoryFace::Instance()->GetTracker());
    
    CFaceTools* pFaceTools = pFace->GetFaceTools();
    if(pFaceTools)
    {
        LoadObject(pFaceTools, pRoot);
    } else
        LoadObject(CFactoryFace::Instance()->GetFaceTools());
    
    LoadObject(CFactoryFace::Instance()->GetDatabase(), pRoot);

    check = connect(&m_Model, SIGNAL(itemChanged(QStandardItem *)),
                    this, SLOT(slotItemChanged(QStandardItem*)));
    Q_ASSERT(check);
    return 0;
}

QStandardItem* CFrmPara::LoadObject(QObject *pObject, QStandardItem *pRoot)
{
    if(!pObject) return nullptr;

    const QMetaObject *pMO = pObject->metaObject();

    QStandardItem *pClass = new QStandardItem(pMO->className());
    pClass->setEditable(false);

    if(!pRoot)
    {
    m_Model.appendRow(pClass);
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    m_Model.itemFromIndex(pClass->index().siblingAtColumn(1))->setEditable(false);
#else
    m_Model.itemFromIndex(pClass->index().sibling(pClass->index().row(), 1))->setEditable(false);
#endif
    } else {
        pRoot->appendRow(pClass);
    }
    
    int nCount = pMO->propertyCount();
    for(int i = 0; i < nCount; i++)
    {
        QMetaProperty p = pMO->property(i);
        if(!p.isValid())
            continue;

        QString szName(p.name());
        QStandardItem* pProperty = new QStandardItem(szName);
        pProperty->setEditable(false);
        pClass->appendRow(pProperty);

        QStandardItem* pValue = new QStandardItem();
        pClass->setChild(pProperty->index().row(), 1,  pValue);
        QVariant value = p.read(pObject);
        pValue->setData(value, Qt::EditRole);
        pValue->setEditable(p.isWritable());

        pValue->setData(szName, CDelegateParamter::ROLE_PROPERTY_NAME);

        QVariant obj;
        obj.setValue(pObject);
        pValue->setData(obj, CDelegateParamter::ROLE_OBJECT);

        pValue->setData(CDelegateParamter::TYPE_OTHER,
                        CDelegateParamter::ROLE_PROPERTY_TYPE);
        if(p.isEnumType() || p.isFlagType())
        {
            int curValue = 0;
            QString szEnum;
            QMetaEnum em = p.enumerator();
            for(int j = 0; j < em.keyCount(); j++)
            {
                QString szVal;
                szEnum += em.key(j) + QString("=")
                        + szVal.setNum(em.value(j)) + ";";
                if(em.value(i) == value.toInt())
                    curValue = i;
            }

            pValue->setData(em.key(curValue), Qt::DisplayRole);
            pValue->setData(szEnum, CDelegateParamter::ROLE_PROPERTY_VALUE);
            pValue->setData(CDelegateParamter::TYPE_ENUM,
                            CDelegateParamter::ROLE_PROPERTY_TYPE);
            continue;
        }
        
        QRegExp rx(".*[Pp]ath.*");
        if(rx.exactMatch(szName))
        {
            pValue->setData(CDelegateParamter::TYPE_DIRECTORY,
                            CDelegateParamter::ROLE_PROPERTY_TYPE);
            continue;
        }
        
        rx = QRegExp(".*[Ff]ile.*");
        if(rx.exactMatch(szName))
        {
            pValue->setData(CDelegateParamter::TYPE_FILE,
                            CDelegateParamter::ROLE_PROPERTY_TYPE);
            continue;
        }
    }
    
    return pClass;
}

void CFrmPara::slotItemChanged(QStandardItem* item)
{
    qDebug() << "CFrmPara::slotItemChanged";
    QObject* pObject = item->data(CDelegateParamter::ROLE_OBJECT)
            .value<QObject*>();
    if(pObject)
    {
        pObject->setProperty(
                item->data(CDelegateParamter::ROLE_PROPERTY_NAME)
                    .toString().toStdString().c_str(),
                item->data(Qt::EditRole));
    }
}
