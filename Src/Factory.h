#ifndef CFACTORY_H_KL_2019_10_21
#define CFACTORY_H_KL_2019_10_21

#pragma once

#include "facerecognizer_export.h"
#include "Face.h"

class FACERECOGNIZER_EXPORT CFactory : public QObject
{
    Q_OBJECT
public:
    CFactory(QObject* parent = nullptr);
    virtual ~CFactory();
    
    static CFace* Instance();
};

#endif // CFACTORY_H_KL_2019_10_21
