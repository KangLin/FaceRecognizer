#include "FaceSeeta.h"
#include <QDir>
#include "Log.h"
#include "ParameterFactory.h"

CFaceSeeta::CFaceSeeta(QObject *parent) : CFace(parent)
{
    Q_UNUSED(parent)
    CParameterFactory* pFactory = CParameterFactory::Instance();
    m_pDetector = new CDetectorSeeta();
    m_pDetector->SetParameter(pFactory->GetParameterDetector());
    m_pTracker = new CTrackerSeeta();
    m_pTracker->SetParameter(pFactory->GetParameterDetector());
    m_pLandmark = new CLandmarkerSeeta();
    m_pLandmark->SetParameter(pFactory->GetParameterLandmark());
    m_pRecognizer = new CRecognizerSeeta();
    m_pRecognizer->SetParameter(pFactory->GetParameterRecognizer());
    m_pFaceTools = new CFaceToolsSeeta();
    m_pFaceTools->SetParameter(pFactory->GetParameterFaceTools());
}

CFaceSeeta::~CFaceSeeta()
{
    if(m_pDetector)
        delete m_pDetector;
    if(m_pTracker)
        delete m_pTracker;
    if(m_pLandmark)
        delete m_pLandmark;
    if(m_pRecognizer)
        delete m_pRecognizer;
    if(m_pFaceTools) delete m_pFaceTools;
}

CDetector* CFaceSeeta::GetDector()
{
    return m_pDetector;
}

CLandmarker* CFaceSeeta::GetLandmarker()
{
    return m_pLandmark;
}

CRecognizer* CFaceSeeta::GetRecognizer()
{
    return m_pRecognizer;
}

CTracker* CFaceSeeta::GetTracker()
{
    return m_pTracker;
}

CFaceTools* CFaceSeeta::GetFaceTools()
{
    return m_pFaceTools;
}
