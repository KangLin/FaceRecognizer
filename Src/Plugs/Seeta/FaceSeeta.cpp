#include "FaceSeeta.h"

#include "Log.h"
#include "DetectorSeeta.h"
#include "TrackerSeeta.h"
#include "LandmarkerSeeta.h"
#include "RecognizerSeeta.h"
#include "FaceToolsSeeta.h"
#include "FactoryFace.h"

#include <QDir>

CFaceSeeta::CFaceSeeta(QObject *parent) : CFace(parent)
{
    Q_UNUSED(parent)
    m_pDetector = new CDetectorSeeta(this);
    m_pTracker = new CTrackerSeeta(this);
    m_pLandmarker = new CLandmarkerSeeta(this);
    m_pRecognizer = new CRecognizerSeeta(this);
    m_pFaceTools = new CFaceToolsSeeta(this);
}

CFaceSeeta::~CFaceSeeta()
{}

int CFaceSeeta::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RegisterFace("Seeta", this, tr("Seeta"));
    return nRet;
}

int CFaceSeeta::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("Seeta");
    return nRet;
}
