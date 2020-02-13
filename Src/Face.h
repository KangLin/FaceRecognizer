#ifndef CFACE_H
#define CFACE_H

#include <QObject>

#include "facerecognizer_export.h"

#include "Detector.h"
#include "Tracker.h"
#include "Landmarker.h"
#include "Recognizer.h"
#include "FaceTools.h"
#include "Parameter.h"
#include "database/Database.h"

class FACERECOGNIZER_EXPORT CFace : public QObject
{
    Q_OBJECT
public:
    explicit CFace(QObject *parent = nullptr);
    virtual ~CFace();
    
    bool IsValid();

    virtual CDetector* GetDector();
    virtual CTracker* GetTracker();
    virtual CLandmarker* GetLandmarker();
    virtual CRecognizer* GetRecognizer();
    virtual CFaceTools* GetFaceTools();
    virtual CDatabase* GetDatabase();
    
protected:
    CDetector* m_pDetector;
    CTracker* m_pTracker;
    CLandmarker* m_pLandmarker;
    CRecognizer* m_pRecognizer;
    CFaceTools* m_pFaceTools;

private:
    CDatabase* m_pDatabase;
};

#endif // CFACE_H
