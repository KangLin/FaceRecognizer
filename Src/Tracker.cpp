#include "Tracker.h"

CTracker::CTracker(QObject *parent) : QObject(parent)
{}

int CTracker::SetParameter(CParameterDetector *pPara)
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

void CTracker::slotParameterDelete()
{
    m_pParameter = nullptr;
}

void CTracker::slotParameterUpdate()
{
    UpdateParameter();
}
