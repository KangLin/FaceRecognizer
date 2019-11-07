#include "ParameterFaceTools.h"

CParameterFaceTools::CParameterFaceTools(QObject *parent) : CParameter(parent)
{
    m_nMinFaceSize = 40;
}

int CParameterFaceTools::SetMinFaceSize(int size)
{
    m_nMinFaceSize = size;
    return 0;
}

int CParameterFaceTools::GetMinFaceSize()
{
    return m_nMinFaceSize;
}
