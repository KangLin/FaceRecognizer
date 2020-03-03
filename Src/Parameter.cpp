#include "Parameter.h"
#include <QDir>
#include "RabbitCommonDir.h"
#include "Log.h"

CParameter::CParameter(QObject *parent) : QObject(parent),
    m_Device(CPU)
{
    QString szPath = RabbitCommon::CDir::Instance()->GetDirData(false)
            + QDir::separator() + "model";
    LOG_MODEL_INFO("CParameter", "szPath:%s", szPath.toStdString().c_str());
#if defined(Q_OS_ANDROID)
    QDir d;
    if(!d.exists(szPath))
    {
        d.mkpath(szPath);
        RabbitCommon::CDir::CopyDirectory(
                    RabbitCommon::CDir::Instance()->GetDirData(true)
                                + QDir::separator() + "model",
                    szPath);
    }
#endif
    SetModelPath(szPath);
}

CParameter::~CParameter()
{}

int CParameter::SetModelPath(const QString &szPath)
{
    m_szModelPath = szPath;
    if(m_szModelPath.right(1) == QDir::separator())
    {
        m_szModelPath = m_szModelPath.left(m_szModelPath.length() - 1);
    }
    emit sigUpdate();
    return 0;
}

QString CParameter::GetModelPath()
{
    return m_szModelPath;
}

int CParameter::SetDevice(DEVICE device)
{
    m_Device = device;
    emit sigUpdate();
    return 0;
}

CParameter::DEVICE CParameter::GetDevice()
{
    return m_Device;
}
