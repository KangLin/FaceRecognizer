#ifndef CPARAMETERDETECTOR_H
#define CPARAMETERDETECTOR_H

#include <QObject>
#include "Parameter.h"

class FACERECOGNIZER_EXPORT CParameterDetector : public CParameter
{
    Q_OBJECT
public:
    explicit CParameterDetector(QObject *parent = nullptr);
    
    int SetFaceSize(int size);
    int GetFaceSize();
    
private:
    int m_nFaceSize;
};

#endif // CPARAMETERDETECTOR_H
