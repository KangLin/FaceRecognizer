#ifndef CTABLEREGISTER_H
#define CTABLEREGISTER_H

#include <QObject>
#include <QSqlDatabase>
#include "DataRegister.h"

class FACERECOGNIZER_EXPORT CTableRegister : public QObject
{
    Q_OBJECT
public:
    explicit CTableRegister(QObject *parent = nullptr);
    
    int SetDatabase(const QSqlDatabase &);
    int Register(qint64 index, CDataRegister *pData);
    int Delete(qint64 index);
    int GetRegisterInfo(qint64 index, CDataRegister *pData);
    bool IsExistNo(qint64 no);
    
private:
    QSqlDatabase m_Database;
};

#endif // CTABLEREGISTER_H
