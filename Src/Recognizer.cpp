#include "Recognizer.h"
#include <QDir>

CRecognizer::CRecognizer(QObject *parent)
    : QObject(parent),
      m_pParameter(nullptr)
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
    
    UpdateParameter();
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

QString CRecognizer::GetRegisterImage(qint64 index)
{
    if(0 > index)
        return m_pParameter->GetRegisterImagePath();
    
    return m_pParameter->GetRegisterImagePath()
            + QDir::separator()
            + QString::number(index) + ".png";
}
