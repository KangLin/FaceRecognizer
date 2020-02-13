#include "FaceSeeta.h"

#include "Log.h"
#include "ParameterFactory.h"
#include "DetectorSeeta.h"
#include "TrackerSeeta.h"
#include "LandmarkerSeeta.h"
#include "RecognizerSeeta.h"
#include "FaceToolsSeeta.h"

#include <QDir>

CFaceSeeta::CFaceSeeta(QObject *parent) : CFace(parent)
{
    Q_UNUSED(parent)
    CParameterFactory* pFactory = CParameterFactory::Instance();
    m_pDetector = new CDetectorSeeta();
    m_pDetector->SetParameter(pFactory->GetParameterDetector());
    m_pTracker = new CTrackerSeeta();
    m_pTracker->SetParameter(pFactory->GetParameterDetector());
    m_pLandmarker = new CLandmarkerSeeta();
    m_pLandmarker->SetParameter(pFactory->GetParameterLandmark());
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
    if(m_pLandmarker)
        delete m_pLandmarker;
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
    return m_pLandmarker;
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
