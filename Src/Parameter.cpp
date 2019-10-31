#include "Parameter.h"
#include <QDir>
#include "RabbitCommonDir.h"

CParameter::CParameter(QObject *parent) : QObject(parent),
    m_Device(CPU)
{
    SetModelPath(RabbitCommon::CDir::Instance()->GetDirApplicationInstallRoot()
            + QDir::separator() + "model" + QDir::separator() + "Seeta");
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
