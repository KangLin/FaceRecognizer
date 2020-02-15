#include "Detector.h"
#include "Log.h"

CDetector::CDetector(CFace *pFace, QObject *parent) : QObject(parent),
    m_pParameter(nullptr),
    m_pFace(pFace)
{}

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
