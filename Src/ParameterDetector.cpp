#include "ParameterDetector.h"

CParameterDetector::CParameterDetector(QObject *parent) : CParameter(parent)
{
    m_nMinFaceSize = 40;
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
