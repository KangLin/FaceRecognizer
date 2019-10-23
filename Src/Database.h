#ifndef CDATABASE_H
#define CDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "DataRegister.h"
#include "TableRegister.h"

class FACERECOGNIZER_EXPORT CDatabase : public QObject
{
    Q_OBJECT
public:
    explicit CDatabase(QObject *parent = nullptr);
    virtual ~CDatabase();

    CTableRegister* GetTableRegister();
    
private:
    int InitDatabase();
    
    QSqlDatabase m_Database;
    CTableRegister m_TableRegister;
};

#endif // CDATABASE_H
