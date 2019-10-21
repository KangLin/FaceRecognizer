#ifndef CPARAMTERFACTORY_H
#define CPARAMTERFACTORY_H

#include <QObject>

#include "ParameterLandmark.h"
#include "ParameterDetector.h"
#include "ParameterRecognizer.h"

class FACERECOGNIZER_EXPORT CParameterFactory : public QObject
{
    Q_OBJECT
public:
    explicit CParameterFactory(QObject *parent = nullptr);
    virtual ~CParameterFactory();
    
    static CParameterFactory* Instance();
    
    enum _PARAMTER_TYPE{
        DETECTORY = 0,
        LANDMARKER,
        RECOGNIZER
    };
    CParameterDetector* GetParameterDetector();
    CParameterLandmark* GetParameterLandmark();
    CParameterRecognizer* GetParameterRecognizer();

    int SetModelPath(const QString &szPath);
    int SetDevice(CParameter::DEVICE device);

private:
    QVector<CParameter*> m_pParameter;
};

#endif // CPARAMTERFACTORY_H
