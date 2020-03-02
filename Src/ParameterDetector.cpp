#include "ParameterDetector.h"

CParameterDetector::CParameterDetector(QObject *parent) : CParameter(parent)
{
#if defined (Q_OS_ANDROID)
    m_nMinFaceSize = 100;
#else
    m_nMinFaceSize = 80;
#endif
}

int CParameterDetector::SetMinFaceSize(int size)
{
    m_nMinFaceSize = size;
    return 0;
}

int CParameterDetector::GetMinFaceSize()
{
    return m_nMinFaceSize;
}
