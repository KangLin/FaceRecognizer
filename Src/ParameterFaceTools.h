#ifndef CPARAMETERFACETOOLS_H
#define CPARAMETERFACETOOLS_H

#include "Parameter.h"

class CParameterFaceTools : public CParameter
{
    Q_OBJECT
public:
    explicit CParameterFaceTools(QObject *parent = nullptr);
    
    int SetMinFaceSize(int size);
    int GetMinFaceSize();
    
private:
    int m_nMinFaceSize;
};

#endif // CPARAMETERFACETOOLS_H
