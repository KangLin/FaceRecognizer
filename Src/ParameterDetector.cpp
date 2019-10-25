#include "ParameterDetector.h"

CParameterDetector::CParameterDetector(QObject *parent) : CParameter(parent)
{
    m_nFaceSize = 40;
}

int CParameterDetector::SetFaceSize(int size)
{
    m_nFaceSize = size;
    return 0;
}

int CParameterDetector::GetFaceSize()
{
    return m_nFaceSize;
}
