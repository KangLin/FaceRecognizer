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
    m_pDetector = new CDetectorSeeta(this);
    m_pDetector->SetParameter(pFactory->GetParameterDetector());
    m_pTracker = new CTrackerSeeta(this);
    m_pTracker->SetParameter(pFactory->GetParameterDetector());
    m_pLandmarker = new CLandmarkerSeeta(this);
    m_pLandmarker->SetParameter(pFactory->GetParameterLandmark());
    m_pRecognizer = new CRecognizerSeeta(this);
    m_pRecognizer->SetParameter(pFactory->GetParameterRecognizer());
    m_pFaceTools = new CFaceToolsSeeta(this);
    m_pFaceTools->SetParameter(pFactory->GetParameterFaceTools());
}

CFaceSeeta::~CFaceSeeta()
{}
