#ifndef CPARAMETERDETECTOR_H
#define CPARAMETERDETECTOR_H

#include <QObject>
#include "Parameter.h"

class FACERECOGNIZER_EXPORT CParameterDetector : public CParameter
{
    Q_OBJECT
public:
    explicit CParameterDetector(QObject *parent = nullptr);
    
    int SetMinFaceSize(int size);
    int GetMinFaceSize();
    
private:
    int m_nMinFaceSize;
};

#endif // CPARAMETERDETECTOR_H
