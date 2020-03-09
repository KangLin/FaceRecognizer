#include "FaceLibfacedetection.h"
#include "DetectorLibfacedetection.h"
#include "ParameterFactory.h"

CFaceLibfacedetection::CFaceLibfacedetection(QObject *parent) : CFace(parent)
{
    CParameterFactory* pFactory = CParameterFactory::Instance();
    m_pDetector = new CDetectorLibfacedetection(this);
    if(m_pDetector) m_pDetector->SetParameter(pFactory->GetParameterDetector());
}
