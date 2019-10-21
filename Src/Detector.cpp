#include "Detector.h"
#include "Log.h"

CDetector::CDetector(QObject *parent) : QObject(parent),
    m_pParameter(nullptr)
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
    UpdateParameter();
}
