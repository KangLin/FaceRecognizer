#include "Tracker.h"
#include "Log.h"

static int gTrackStrFaceId = qRegisterMetaType<CTracker::strFace>();
static int gVectorTrackStrFaceId = qRegisterMetaType<QVector<CTracker::strFace> >();
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
    QString szErr;
    int nRet = -UpdateParameter(szErr);
    if(nRet)
        LOG_MODEL_ERROR("CTracker", "UpdateParameter: %s",
                         szErr.toStdString().c_str());
}
