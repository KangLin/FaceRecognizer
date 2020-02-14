#ifndef CFACTORY_H_KL_2019_10_21
#define CFACTORY_H_KL_2019_10_21

#pragma once

#include "facerecognizer_export.h"
#include "Face.h"

// @defgroup CFactoryFace Face factory

/**
 * @brief The CFactoryFace class
 * @details 此类提供人脸功能接口。
 *          如果使用自动，则使用开发定的接口。
 *          如果仅要指定某个库，则使用SetLibType
 */
class FACERECOGNIZER_EXPORT CFactoryFace : public QObject
{
    Q_OBJECT
public:
    CFactoryFace(QObject* parent = nullptr);
    virtual ~CFactoryFace();

    static CFactoryFace* Instance();
    
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
    int SetLibType(LIB_TYPE type, bool bOnly = true);
    
private:
    CFace* m_Face[AUTO];
    LIB_TYPE m_CurrentLIb;
    bool m_bOnlyUserCurrent;
};

#endif // CFACTORY_H_KL_2019_10_21
