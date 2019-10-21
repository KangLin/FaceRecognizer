#include "Recognizer.h"

CRecognizer::CRecognizer(QObject *parent)
    : QObject(parent),
      m_pParameter(nullptr),
      m_nCount(0)
{}

CRecognizer::~CRecognizer()
{}

int CRecognizer::SetParameter(CParameterRecognizer *pPara)
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

void CRecognizer::slotParameterDelete()
{
    m_pParameter = nullptr;
}

void CRecognizer::slotParameterUpdate()
{
    UpdateParameter();
}

qint64 CRecognizer::GetCount()
{
    return m_nCount;
}

int CRecognizer::SetCount(qint64 count)
{
    m_nCount = count;
    return 0;
}
