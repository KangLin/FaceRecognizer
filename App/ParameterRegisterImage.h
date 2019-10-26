#ifndef CPARAMETERREGISTERIMAGE_H
#define CPARAMETERREGISTERIMAGE_H

#include <QObject>
#include <QImage>

class CParameterRegisterImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 no READ GetNo WRITE SetNo)
    Q_PROPERTY(QString name READ GetName WRITE SetName)
    Q_PROPERTY(QImage image READ GetImage WRITE SetImage)
    
public:
    explicit CParameterRegisterImage(QObject *parent = nullptr);
    CParameterRegisterImage(const CParameterRegisterImage &othe);
    
    int SetNo(qint64 no);
    qint64 GetNo();
    
    int SetName(const QString &szName);
    QString GetName();
    
    int SetImage(const QImage &image);
    QImage GetImage();
    
private:
    qint64 m_No;
    QString m_szName;
    QImage m_Image;
};

#endif // CPARAMETERREGISTERIMAGE_H
