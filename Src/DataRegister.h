#ifndef CDATAREGISTER_H
#define CDATAREGISTER_H

#include <QObject>
#include "facerecognizer_export.h"

class FACERECOGNIZER_EXPORT CDataRegister : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 idx READ getIdx WRITE setIdx)
    Q_PROPERTY(qint64 no READ getNo WRITE setNo)
    Q_PROPERTY(QString name READ getName WRITE setName)
    
public:
    explicit CDataRegister(QObject *parent = nullptr);
    
    qint64 getIdx();
    int setIdx(qint64 idx);

    qint64 getNo();
    int setNo(qint64 nNo);
    
    QString getName();
    int setName(const QString &szName);
    
private:
    qint64 m_nIdx;
    qint64 m_nNo;
    QString m_szName;
};

#endif // CDATAREGISTER_H
