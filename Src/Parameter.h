#ifndef CPARAMETER_H
#define CPARAMETER_H

#include <QObject>
#include "facerecognizer_export.h"

class FACERECOGNIZER_EXPORT CParameter : public QObject
{
    Q_OBJECT
public:
    explicit CParameter(QObject *parent = nullptr);

    int SetModelPath(const QString &szPath);
    QString GetModelPath();
    
    enum DEVICE
    {
        AUTO,
        CPU,
        GPU
    };
    int SetDevice(DEVICE device);
    DEVICE GetDevice();
    
Q_SIGNALS:
    void sigUpdate();
    
private:
    QString m_szModelPath;
    DEVICE m_Device;
};

#endif // CPARAMETER_H
