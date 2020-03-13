#ifndef CFACTORY_H_KL_2019_10_21
#define CFACTORY_H_KL_2019_10_21

#pragma once

#include "facerecognizer_export.h"
#include "Face.h"

// @defgroup CFactoryFace Face factory

/**
 * @brief The CFactoryFace class
 * @details 此类提供人脸功能接口。
 *     1. 如果使用自动，则使用开发定的接口。
 *        CFactoryFace* pFace = CFactoryFace::Instance();
 *     2. 如果仅要指定某个库，则使用 SetLibType
 *
 *        例如仅使用 SEETA ：
 * 
 *        CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType(SEETA, true);
 *        或者：
 *        CFace* pFace = CFactoryFace::Instance()->GetFace(SEETA);
 * 
 *     3. 当指定的某个库接口没有完全实现时，自动使用其他库的接口
 *        例如开发时使用 OPENCV ，但他有接口没有实现：
 *        CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType(OPENCV, false);
 */
class FACERECOGNIZER_EXPORT CFactoryFace : public QObject
{
    Q_OBJECT

#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
    Q_ENUMS((LIB_TYPE)
#endif
public:
    CFactoryFace(QObject* parent = nullptr);
    virtual ~CFactoryFace();

    static CFactoryFace* Instance();
    
    enum LIB_TYPE{
        SEETA,
        OPENCV,
        DLIB,
        LIBFACEDETECTION,
        AUTO
    };
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    Q_ENUM(LIB_TYPE)
#endif
    int SetLibType(LIB_TYPE type, bool bOnly = true);
    
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
    LIB_TYPE m_CurrentLib;
    bool m_bOnlyUserCurrent;
};

#endif // CFACTORY_H_KL_2019_10_21
