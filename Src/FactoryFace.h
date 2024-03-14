#ifndef CFACTORY_H_KL_2019_10_21
#define CFACTORY_H_KL_2019_10_21

#pragma once

#include "facerecognizer_export.h"
#include "Face.h"

#include <QMetaClassInfo>
#include <QDir>
#include <QVector>

/*!
 * \~english
 * \defgroup API Application interface
 * \defgroup API_INTERNAL Internal interface
 *
 * \~chinse
 * \defgroup API 接口
 * \defgroup API_INTERNAL 内部接口
 */

/**
 * \~english
 * \brief Face recognition interface factory.
 *        Used to manage instances of face recognition API plug-ins.
 *        Used by clients only.
 * \details This class provides a face function interface.
 *  1. If you use automatic, use the interface that is customized.
 *     CFactoryFace* pFace = CFactoryFace::Instance();
 *  2. If you want to specify only a certain library, use SetLibType
 *     For example, use SEETA :
 *     \code
 *     CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType("Seeta", true);
 *     \endcode
 *     Or:
 *     \code
 *     CFace* pFace = CFactoryFace::Instance()->GetFace(SEETA);
 *     \endcode
 *  3. When a specified library interface is not fully implemented,
 *     the interface of another library is automatically used
 *     For example, we use opencv for development,
 *     but we have an interface that we don't implement:
 *     \code
 *     CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType("OpenCV", false);
 *     \endcode
 *
 * \~chinese
 * \brief 人脸识别接口类厂。用于管理人脸识别接口插件实例。仅由客户端使用。
 * 
 * \details 此类提供人脸功能接口。
 *     1. 如果使用自动，则使用开发定的接口。
 *        \code
 *        CFactoryFace* pFace = CFactoryFace::Instance();
 *        \endcode
 *     2. 如果仅要指定某个库，则使用 SetLibType
 *
 *        例如仅使用 SEETA ：
 *        \code
 *        CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType("Seeta", true);
 *        \endcode
 *        或者：
 *        \code
 *        CFace* pFace = CFactoryFace::Instance()->GetFace(SEETA);
 *        \endcode
 * 
 *     3. 当指定的某个库接口没有完全实现时，自动使用其他库的接口
 *        例如开发时使用 OPENCV ，但他有接口没有实现：
 *        \code
 *        CFactoryFace* pFace = CFactoryFace::Instance()->SetLibType("OpenCV", false);
 *        \endcode
 * \~
 * \see CFace
 * \ingroup API
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
    
    virtual const QString Detail() const;

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
