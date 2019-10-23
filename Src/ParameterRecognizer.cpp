#include "ParameterRecognizer.h"
#include "RabbitCommonDir.h"
#include <QDir>

CParameterRecognizer::CParameterRecognizer(QObject *parent) : CParameter(parent)
{
    SetFeatureFile(RabbitCommon::CDir::Instance()->GetDirUserData()
            + QDir::separator()
            + "feature.db");
    SetRegisterImagePath(RabbitCommon::CDir::Instance()->GetDirUserImage());
}

int CParameterRecognizer::SetFeatureFile(const QString &szFile)
{
    m_szFeatureFile = szFile;
    return 0;
}

QString CParameterRecognizer::GetFeatureFile()
{
    return m_szFeatureFile;
}

int CParameterRecognizer::SetRegisterImagePath(const QString &szPath)
{
    m_szRegisterImagePath = szPath;
    QDir d(m_szRegisterImagePath);
    if(!d.exists(m_szRegisterImagePath))
        d.mkpath(m_szRegisterImagePath);
    return 0;
}

QString CParameterRecognizer::GetRegisterImagePath()
{
    return m_szRegisterImagePath;
}
