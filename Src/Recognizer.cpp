#include "Recognizer.h"
#include <QDir>
#include "RabbitCommonDir.h"

CRecognizer::CRecognizer(CFace *pFace, QObject *parent)
    : CFaceBase(parent),
      m_pFace(pFace),
      m_bInit(false)
{
    m_szImagePath = RabbitCommon::CDir::Instance()->GetDirUserImage();
    QDir d(m_szImagePath);
    if(!d.exists(m_szImagePath))
        d.mkpath(m_szImagePath);
}

CRecognizer::~CRecognizer()
{}

QString CRecognizer::GetRegisterImage(qint64 index)
{
    if(0 > index)
        return getImagePath();

    return getImagePath()
            + QDir::separator()
            + QString::number(index) + ".png";
}

bool CRecognizer::IsValid()
{
    return m_bInit;
}

QString CRecognizer::getImagePath()
{
    return m_szImagePath;
}

int CRecognizer::setImagePath(const QString &szPath)
{
    m_szImagePath = szPath;
    return 0;
}
