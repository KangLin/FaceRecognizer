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

    static CFactory* Instance();
    
    enum LIB_TYPE{
        OPENCV,
        SEETA,
        DLIB,
        AUTO
    };

    virtual CFace* GetFace(LIB_TYPE type = AUTO);
    virtual CDetector* GetDector(LIB_TYPE type = AUTO);
    virtual CTracker* GetTracker(LIB_TYPE type = AUTO);
    virtual CLandmarker* GetLandmarker(LIB_TYPE type = AUTO);
    virtual CRecognizer* GetRecognizer(LIB_TYPE type = AUTO);
    virtual CFaceTools* GetFaceTools(LIB_TYPE type = AUTO);
    virtual CDatabase* GetDatabase(LIB_TYPE type = AUTO);
    
    bool bIsValid(LIB_TYPE type = AUTO);
    
private:
    CFace* m_Face[AUTO];
};

#endif // CFACTORY_H_KL_2019_10_21
