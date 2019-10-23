#ifndef CDATAREGISTER_H
#define CDATAREGISTER_H

#include <QObject>

class CDataRegister : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 index READ getIndex WRITE setIndex)
    Q_PROPERTY(qint64 no READ getNo WRITE setNo)
    Q_PROPERTY(QString name READ getName WRITE setName)
    
public:
    explicit CDataRegister(QObject *parent = nullptr);
    
    qint64 getIndex();
    int setIndex(qint64 index);

    qint64 getNo();
    int setNo(qint64 nNo);
    
    QString getName();
    int setName(const QString &szName);
    
private:
    qint64 m_nIndex;
    qint64 m_nNo;
    QString m_szName;
};

#endif // CDATAREGISTER_H
