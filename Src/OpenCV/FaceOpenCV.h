#ifndef FACEOPENCV_H
#define FACEOPENCV_H

#include "Face.h"

class CFaceOpenCV : public CFace
{
    Q_OBJECT
public:
    explicit CFaceOpenCV(QObject *parent = nullptr);
    virtual ~CFaceOpenCV();
    
    virtual CDetector* GetDector();
    virtual CTracker* GetTracker();
    virtual CLandmarker* GetLandmarker();
    virtual CRecognizer* GetRecognizer();
    virtual CFaceTools* GetFaceTools();
    
private:
    CDetector* m_pDetector;
};

#endif // FACEOPENCV_H
