#ifndef CFACE_H
#define CFACE_H

#include <QObject>

#include "facerecognizer_export.h"

#include "Detector.h"
#include "Landmarker.h"
#include "Recognizer.h"
#include "Parameter.h"

class FACERECOGNIZER_EXPORT CFace : public QObject
{
    Q_OBJECT
public:
    explicit CFace(QObject *parent = nullptr);
    
    virtual CDetector* GetDector() = 0;
    virtual CLandmarker* GetLandmarker() = 0;
    virtual CRecognizer* GetRecognizer() = 0;
};

#endif // CFACE_H
