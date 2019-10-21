#include "ParameterRecognizer.h"
#include "RabbitCommonDir.h"
#include <QDir>

CParameterRecognizer::CParameterRecognizer(QObject *parent) : CParameter(parent)
{
    m_szFeatureFile = RabbitCommon::CDir::Instance()->GetDirUserData()
            + QDir::separator()
            + "feature.db";
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
