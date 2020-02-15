#include "FaceTools.h"
#include "Log.h"

CFaceTools::CFaceTools(CFace *pFace, QObject *parent) : QObject(parent),
    m_pParameter(nullptr),
    m_pFace(pFace)
{}
    
int CFaceTools::SetParameter(CParameterFaceTools *pPara)
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

void CFaceTools::slotParameterDelete()
{
    m_pParameter = nullptr;
}

void CFaceTools::slotParameterUpdate()
{
    QString szErr;
    int nRet = -UpdateParameter(szErr);
    if(nRet)
        LOG_MODEL_ERROR("CFaceTools", "UpdateParameter: %s",
                         szErr.toStdString().c_str());
}
