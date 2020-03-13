#include "Detector.h"
#include "Log.h"
#include "RabbitCommonDir.h"

CDetector::CDetector(CFace *pFace, QObject *parent) : QObject(parent),
    m_pParameter(nullptr),
    m_pFace(pFace)
{
    m_szModelPath = RabbitCommon::CDir::Instance()->GetDirData(false)
            + QDir::separator() + "model";
    LOG_MODEL_INFO("CParameter", "szPath:%s", m_szModelPath.toStdString().c_str());
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

CDetector::~CDetector()
{}

int CDetector::SetParameter(CParameterDetector *pPara)
{
    if(!pPara)
        return -1;
    
    m_pParameter = pPara;
    bool bCheck = connect(m_pParameter, SIGNAL(destroyed()),
                          this, SLOT(slotParameterDelete()));
    Q_ASSERT(bCheck);
    bCheck = connect(m_pParameter, SIGNAL(sigUpdate()),
                     this, SLOT(slotParameterUpdate()));
    Q_ASSERT(bCheck);
    
    emit m_pParameter->sigUpdate();
    return 0;
}

void CDetector::slotParameterDelete()
{
    m_pParameter = nullptr;
}

void CDetector::slotParameterUpdate()
{
    QString szErr;
    int nRet = UpdateParameter(szErr);
    if(nRet)
        LOG_MODEL_ERROR("CDetector", "UpdateParameter: %s",
                         szErr.toStdString().c_str());
}

int CDetector::setModelPath(const QString &szPath)
{
    m_szModelPath = szPath;
    return 0;
}

QString CDetector::getModelPath()
{
    return m_szModelPath;
}
