#include "FaceBase.h"

#include "RabbitCommonDir.h"
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "Face.Base")

CFaceBase::CFaceBase(QObject *parent) : QObject(parent),
    m_Device(CPU)
{
    m_szModelPath = RabbitCommon::CDir::Instance()->GetDirData(false)
            + QDir::separator() + "model";
    qDebug(log) << "szPath:" << m_szModelPath;
#if defined(Q_OS_ANDROID)
    QDir d;
    if(!d.exists(m_szModelPath))
    {
        d.mkpath(m_szModelPath);
        RabbitCommon::CDir::CopyDirectory(
                    RabbitCommon::CDir::Instance()->GetDirData(true)
                                + QDir::separator() + "model",
                    m_szModelPath);
    }
#endif
}

int CFaceBase::setModelPath(const QString &szPath)
{
    if(m_szModelPath == szPath)
        return 0;
    m_szModelPath = szPath;
    return UpdateParameter();
}

QString CFaceBase::getModelPath()
{
    return m_szModelPath;
}

CFaceBase::DEVICE CFaceBase::getDevice()
{
    return m_Device;
}

int CFaceBase::setDevice(DEVICE device)
{
    if(m_Device == device)
        return 0;
    m_Device = device;
    return UpdateParameter();
}
