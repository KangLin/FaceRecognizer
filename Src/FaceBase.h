#ifndef CFACEBASE_H
#define CFACEBASE_H

#include <QObject>

class CFaceBase : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
    Q_PROPERTY(QString modelPath READ getModelPath WRITE setModelPath)
    Q_PROPERTY(DEVICE device READ getDevice WRITE setDevice)
    
public:
    explicit CFaceBase(QObject *parent = nullptr);
    
    virtual int setModelPath(const QString& szPath);
    virtual QString getModelPath();

    enum DEVICE
    {
        CPU,
        GPU
    };
    Q_ENUM(DEVICE)
    virtual DEVICE getDevice();
    virtual int setDevice(DEVICE device);
    
private:
    QString m_szModelPath;
    DEVICE m_Device;

    virtual int UpdateParameter() = 0; 
};

#endif // CFACEBASE_H
