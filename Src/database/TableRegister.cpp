#include "TableRegister.h"
#include "RabbitCommonDir.h"
#include "Log.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QMetaProperty>
#include <QMetaClassInfo>
#include <QVariant>

CTableRegister::CTableRegister(QObject *parent) : QObject(parent)
{
}

int CTableRegister::SetDatabase(const QSqlDatabase &db)
{
    m_Database = db;
    return 0;
}

int CTableRegister::Register(qint64 index, CDataRegister *pData)
{
    int nRet = 0;
    if(!pData)
        return -1;
    if(pData->getIdx() != index)
        return -2;

    QString szCol, szValue;
    int nCount = pData->metaObject()->propertyCount();
    for(int i = pData->metaObject()->propertyOffset(); i < nCount; i++)
    {
        QMetaProperty p = pData->metaObject()->property(i);
        if(!(p.isReadable() && p.isWritable()))
            continue;
        
        if(pData->metaObject()->propertyOffset() == i)
        {
            szCol += p.name();
            szValue = "'" + p.read(pData).toString() + "'";
        }
        else
        {
            szCol += ", ";
            szCol += p.name();
            szValue += ", '";
            szValue += p.read(pData).toString() + "'";
        }
    }

    QString szSql = "INSERT INTO Register (" + szCol + ") VALUES (" + szValue + ");";
    LOG_MODEL_DEBUG("CDatabase", "sql: %s\n", szSql.toStdString().c_str());
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
    {
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
        return m_Database.lastError().nativeErrorCode().toInt();
    }
    return nRet;
}

int CTableRegister::Delete(qint64 index)
{
    int nRet = 0;
    QString szSql = "DELETE FROM Register WHERE idx="
            + QString::number(index) + ";";
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
    {
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
        return m_Database.lastError().nativeErrorCode().toInt();
    }
    return nRet;
}

int CTableRegister::GetRegisterInfo(qint64 index, CDataRegister *pData)
{
    int nRet = 0;
    if(!pData)
        return -1;

    QString szSql = "SELECT * FROM Register WHERE idx="
            + QString::number(index) + ";";
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
    {
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
        return m_Database.lastError().nativeErrorCode().toInt();
    }
    while (query.next()) {
        int nCount = pData->metaObject()->propertyCount();
        for(int i = pData->metaObject()->propertyOffset(); i < nCount; i++)
        {
            QMetaProperty p = pData->metaObject()->property(i);
            if(p.isWritable())
                p.write(pData, query.value(p.name()));
            
            QString szName = p.name();
            int idx = query.record().indexOf(p.name());
            QString szValue = query.value(idx).toString();
        }
    }
    return nRet;
}

bool CTableRegister::IsExistNo(qint64 no)
{
    QString szSql;
    if(-1 == no)
        szSql = "SELECT * FROM Register";
    else
        szSql = "SELECT * FROM Register WHERE no="
                + QString::number(no) + ";";
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
    {
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
        return false;
    }
    if(query.next())
        return true;
    return false;
}
