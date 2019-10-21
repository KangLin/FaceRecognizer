#include "ParameterLandmark.h"

CParameterLandmark::CParameterLandmark(QObject *parent) : CParameter(parent)
{
    m_Points = 5;
}

int CParameterLandmark::SetPoints(int points)
{
    m_Points = points;
    emit sigUpdate();
    return 0;
}

int CParameterLandmark::GetPoints()
{
    return m_Points;
}
