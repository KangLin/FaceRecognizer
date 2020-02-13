#include "FaceOpenCV.h"
#include "ParameterFactory.h"
#include "DetectorOpenCV.h"

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent),
    m_pDetector(nullptr),
    m_pTracker(nullptr),
    m_pLandmarker(nullptr),
    m_pRecognizer(nullptr),
    m_pFaceTools(nullptr)
{
    CParameterFactory* pFactory = CParameterFactory::Instance();
#ifdef HAVE_OPENCV_DNN
    m_pDetector = new CDetectorOpenCV();
    if(m_pDetector) m_pDetector->SetParameter(pFactory->GetParameterDetector());
#endif
}

CFaceOpenCV::~CFaceOpenCV()
{
    if(m_pDetector)
        delete m_pDetector;
    if(m_pTracker)
        delete m_pTracker;
    if(m_pLandmarker)
        delete m_pLandmarker;
    if(m_pRecognizer)
        delete m_pRecognizer;
    if(m_pFaceTools) delete m_pFaceTools;
}

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
