#include "Landmarker.h"

CLandmarker::CLandmarker(QObject *parent)
    : QObject(parent),
      m_pParameter(nullptr)
{}

CLandmarker::~CLandmarker()
{}

int CLandmarker::SetParameter(CParameterLandmark *pPara)
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

void CLandmarker::slotParameterDelete()
{
    m_pParameter = nullptr;
}

void CLandmarker::slotParameterUpdate()
{
    UpdateParameter();
}
