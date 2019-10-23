#include "Database.h"
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

CDatabase::CDatabase(QObject *parent)
    : QObject(parent)
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

    m_TableRegister.SetDatabase(m_Database);
    return 0;
}

CTableRegister* CDatabase::GetTableRegister()
{
    return &m_TableRegister;
}
