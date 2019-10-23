#include "Database.h"
#include "RabbitCommonDir.h"
#include "Log.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QMetaProperty>
#include <QMetaClassInfo>
#include <QVariant>

CDatabase::CDatabase(QObject *parent) : QObject(parent)
{
    InitDatabase();
}

CDatabase::~CDatabase()
{
    m_Database.close();
}

int CDatabase::InitDatabase()
{
    QString szFile;
    szFile = RabbitCommon::CDir::Instance()->GetDirUserDatabaseFile();
    
    m_Database = QSqlDatabase::addDatabase("QSQLITE");
    m_Database.setDatabaseName(szFile);
    QDir d;
    if(!d.exists(szFile))
    {
        if(!m_Database.open())
        {
            LOG_MODEL_ERROR("CDatabase", "Open database fail: %s",
                       m_Database.lastError().text().toStdString().c_str());
            return m_Database.lastError().number();
        }
#if defined (_DEBUG) || defined(DEBUG)
        QFile file(":/database/database");
#else
        QFile file(RabbitCommon::CDir::Instance()->GetDirDatabase()
                   + QDir::separator() + "database.sql");
#endif
        if(file.open(QFile::ReadOnly))
        {
            QString szSql(file.readAll());
            QSqlQuery query(m_Database);
            if(!query.exec(szSql))
                LOG_MODEL_ERROR("CDatabase", "Create database fail: %s",
                           m_Database.lastError().text().toStdString().c_str());
            file.close();
        }
        m_Database.close();
    }

    if(!m_Database.open())
    {
        LOG_MODEL_ERROR("CDatabase", "Open database fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
        return -2;
    }

    return 0;
}

int CDatabase::Register(qint64 index, CDataRegister *pData)
{
    int nRet = 0;
    if(!pData)
        return -1;
    if(pData->getIndex() != index)
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

    QString szSql = "INSERT INTO Register (" + szCol + ") VALUES (" + szValue + ")";
    LOG_MODEL_DEBUG("CDatabase", "sql: %s", szSql);
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
    return nRet;
}

int CDatabase::Delete(qint64 index)
{
    int nRet = 0;
    QString szSql = "DELETE FROM Register WHERE index=" + QString::number(index);
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
    return nRet;
}

int CDatabase::GetRegisterInfo(qint64 index, CDataRegister *pData)
{
    int nRet = 0;
    if(!pData)
        return -1;

    QString szSql = "SELECT * FROM Register WHERE index=" + QString::number(index);
    QSqlQuery query(m_Database);
    if(!query.exec(szSql))
        LOG_MODEL_ERROR("CDatabase", "Register fail: %s",
                   m_Database.lastError().text().toStdString().c_str());
    int nCount = pData->metaObject()->propertyCount();
    for(int i = pData->metaObject()->propertyOffset(); i < nCount; i++)
    {
        QMetaProperty p = pData->metaObject()->property(i);
        if(p.isWritable())
            p.write(pData, query.value(p.name()));
    }

    return nRet;
}
