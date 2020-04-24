#include "FaceBase.h"
#include "Log.h"
#include "RabbitCommonDir.h"

CFaceBase::CFaceBase(QObject *parent) : QObject(parent),
    m_Device(CPU)
{
    m_szModelPath = RabbitCommon::CDir::Instance()->GetDirData(false)
            + QDir::separator() + "model";
    LOG_MODEL_DEBUG("CParameter", "szPath:%s", m_szModelPath.toStdString().c_str());
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
