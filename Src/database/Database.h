#ifndef CDATABASE_H
#define CDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "DataRegister.h"
#include "TableRegister.h"

class FACERECOGNIZER_EXPORT CDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dbFile READ getDbFile WRITE setDbFile)
public:
    explicit CDatabase(QObject *parent = nullptr);
    virtual ~CDatabase();

    CTableRegister* GetTableRegister();
    QString getDbFile();
    int setDbFile(const QString &szFile);
    
private:
    int InitDatabase();
    
    QSqlDatabase m_Database;
    CTableRegister m_TableRegister;
    
    QString m_szFile;
};

#endif // CDATABASE_H
