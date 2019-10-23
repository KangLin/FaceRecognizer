#ifndef CDATABASE_H
#define CDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "DataRegister.h"

class CDatabase : public QObject
{
    Q_OBJECT
public:
    explicit CDatabase(QObject *parent = nullptr);
    virtual ~CDatabase();
    
    int Register(qint64 index, CDataRegister *pData);
    int Delete(qint64 index);
    int GetRegisterInfo(qint64 index, CDataRegister *pData);

private:
    int InitDatabase();
    
    QSqlDatabase m_Database;  
};

#endif // CDATABASE_H
