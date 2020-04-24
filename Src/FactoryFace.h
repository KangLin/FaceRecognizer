#ifndef CFACTORY_H_KL_2019_10_21
#define CFACTORY_H_KL_2019_10_21

#pragma once

#include "facerecognizer_export.h"
#include "Face.h"

#include <QMetaClassInfo>
#include <QDir>
#include <QVector>

// @defgroup CFactoryFace Face factory

/**
 * @brief The CFactoryFace class
 * 
 * @details 此类提供人脸功能接口。
 *     1. 如果使用自动，则使用开发定的接口。
 *        CFactoryFace* pFace = CFactoryFace::Instance();
 *     2. 如果仅要指定某个库，则使用 SetLibType
 *
 *        例如仅使用 SEETA ：
 * 
 *        CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType("Seeta", true);
 *        或者：
 *        CFace* pFace = CFactoryFace::Instance()->GetFace(SEETA);
 * 
 *     3. 当指定的某个库接口没有完全实现时，自动使用其他库的接口
 *        例如开发时使用 OPENCV ，但他有接口没有实现：
 *        CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType("OpenCV", false);
 * 
 * @see CFace
 */
class FACERECOGNIZER_EXPORT CFactoryFace : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
    Q_ENUMS((LIB_TYPE)
#endif
public:
    CFactoryFace(QObject* parent = nullptr);
    virtual ~CFactoryFace();

    static CFactoryFace* Instance();

    int SetLibType(const QString &szName = QString(), bool bOnly = true);
    QVector<CFace*> GetLibType();

    virtual CFace* GetFace(const QString &szName = QString());

    virtual CDetector* GetDector(const QString &szName = QString());
    virtual CTracker* GetTracker(const QString &szName = QString());
    virtual CLandmarker* GetLandmarker(const QString &szName = QString());
    virtual CRecognizer* GetRecognizer(const QString &szName = QString());
    virtual CFaceTools* GetFaceTools(const QString &szName = QString());
    virtual CDatabase* GetDatabase(const QString &szName = QString());
    
    bool bIsValid(const QString &szName = QString());
    
    int setModelPath(const QString &szPath);
    
private:
    int m_CurrentLib;
    bool m_bOnlyUserCurrent;
    QVector<CFace*> m_Faces;
    
private:
    /**
     * @brief RegisterFace
     * @param szName
     * @param pFace
     * @param szDescript
     * @return
     */
    virtual int RegisterFace(CFace* pFace);
    virtual int RemoveFace(const QString &szName, CFace* pFace = nullptr);
    int FindPlugins(QDir dir, QStringList filters);
};

#endif // CFACTORY_H_KL_2019_10_21
