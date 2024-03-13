#include "FaceSeeta2.h"

#include "DetectorSeeta.h"
#include "TrackerSeeta.h"
#include "LandmarkerSeeta.h"
#include "RecognizerSeeta.h"
#include "FaceToolsSeeta.h"

#include <QDir>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "Plugin.Seeta.Face")

CFaceSeeta2::CFaceSeeta2(QObject *parent) : CFace(parent)
{}

CFaceSeeta2::~CFaceSeeta2()
{}

int CFaceSeeta2::Initialize()
{
    m_pDetector = new CDetectorSeeta(this);
    m_pTracker = new CTrackerSeeta(this);
    m_pLandmarker = new CLandmarkerSeeta(this);
    m_pRecognizer = new CRecognizerSeeta(this);
    m_pFaceTools = new CFaceToolsSeeta(this);
    return 0;
}

QString CFaceSeeta2::GetName()
{
    return "SeetaFace2";
}

int CFaceSeeta2::GetLevel()
{
    return 10;
}

const QString CFaceSeeta2::Detail() const
{
    QString szDetail;
    szDetail = tr("### Seeta2") + "\n";
    if(m_pDetector)
        szDetail += tr("- Detector");
    if(m_pRecognizer)
        szDetail += tr("- Recognizer");
    if(m_pLandmarker)
        szDetail += tr("- Landmarker");
    if(m_pTracker)
        szDetail += tr("- Tracker");
    if(m_pFaceTools)
        szDetail += tr("- Face tools");
    return szDetail;
}
