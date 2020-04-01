#include "FaceSeeta2.h"

#include "Log.h"
#include "DetectorSeeta.h"
#include "TrackerSeeta.h"
#include "LandmarkerSeeta.h"
#include "RecognizerSeeta.h"
#include "FaceToolsSeeta.h"

#include <QDir>

CFaceSeeta2::CFaceSeeta2(QObject *parent) : CFace(parent)
{
    Q_UNUSED(parent)
    m_pDetector = new CDetectorSeeta(this);
    m_pTracker = new CTrackerSeeta(this);
    m_pLandmarker = new CLandmarkerSeeta(this);
    m_pRecognizer = new CRecognizerSeeta(this);
    m_pFaceTools = new CFaceToolsSeeta(this);
}

CFaceSeeta2::~CFaceSeeta2()
{}

QString CFaceSeeta2::GetName()
{
    return "SeetaFace2";
}

int CFaceSeeta2::GetLevel()
{
    return 10;
}
