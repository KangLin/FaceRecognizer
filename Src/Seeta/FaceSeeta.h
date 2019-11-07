#ifndef CFACESEETA_H_KL_2019_10_21
#define CFACESEETA_H_KL_2019_10_21

#pragma once

#include "Face.h"
#include "DetectorSeeta.h"
#include "TrackerSeeta.h"
#include "LandmarkerSeeta.h"
#include "RecognizerSeeta.h"
#include "FaceToolsSeeta.h"

#include <QSharedPointer>

class CFaceSeeta : public CFace
{
    Q_OBJECT
public:
    CFaceSeeta(QObject* parent = nullptr);
    virtual ~CFaceSeeta();
    
    virtual CDetector* GetDector();
    virtual CTracker* GetTracker();
    virtual CLandmarker* GetLandmarker();
    virtual CRecognizer* GetRecognizer();
    virtual CFaceTools* GetFaceTools();
    
private:
    CDetectorSeeta* m_pDetector;
    CTrackerSeeta* m_pTracker;
    CLandmarkerSeeta* m_pLandmark;
    CRecognizerSeeta* m_pRecognizer;
    CFaceToolsSeeta* m_pFaceTools;
};

#endif // CFACESEETA_H_KL_2019_10_21
