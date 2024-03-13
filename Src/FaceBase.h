#ifndef CFACEBASE_H
#define CFACEBASE_H

#include <QObject>
#include "facerecognizer_export.h"

/*!
 * \~english  API_FACE Face interface
 *
 * \~chinese
 * \defgroup API_FACE 人脸识别接口
 *
 * \~
 * \ingroup API
 */

/**
 * \~english the class of face interface.
 *   it is the base class of all face interface.
 *
 * \~chinese
 * \brief 人脸识别接口类。 它是所有人脸接口的基类
 *
 * \~
 * \ingroup API
 */
class FACERECOGNIZER_EXPORT CFaceBase : public QObject
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

protected:
    virtual int UpdateParameter() = 0;

private:
    QString m_szModelPath;
    DEVICE m_Device;
};

#endif // CFACEBASE_H
