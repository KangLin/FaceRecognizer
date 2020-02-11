#include "FaceOpenCV.h"
#include "ParameterFactory.h"

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent)
{
    CParameterFactory* pFactory = CParameterFactory::Instance();
    m_pDetector = new CDetectorOpenCV();
    m_pDetector->SetParameter(pFactory->GetParameterDetector());
}

CFaceOpenCV::~CFaceOpenCV()
{}

CDetector* CFaceOpenCV::GetDector()
{
    return m_pDetector;
}

CTracker* CFaceOpenCV::GetTracker()
{
    return nullptr;
}

CLandmarker* CFaceOpenCV::GetLandmarker()
{
    return nullptr;
}

CRecognizer* CFaceOpenCV::GetRecognizer()
{
    return nullptr;
}

CFaceTools* CFaceOpenCV::GetFaceTools()
{
    return nullptr;
}
