#ifndef CRECOGNIZER_H_KL_2019_10_21_
#define CRECOGNIZER_H_KL_2019_10_21_
  
#pragma once
#include <QObject>
#include <QImage>
#include <QMetaClassInfo>

/**
 * @defgroup RecognizerInterface recognizer interface
 */

#include "facerecognizer_export.h"
#include "FaceBase.h"

class CFace;

/**
 * @brief The CRecognizer class
 * @ingroup RecognizerInterface
 */
class FACERECOGNIZER_EXPORT CRecognizer : public CFaceBase
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
    Q_PROPERTY(QString imagePath READ getImagePath WRITE setImagePath)
    
public:
    CRecognizer(CFace* pFace = nullptr, QObject* parent = nullptr);
    virtual ~CRecognizer();
    
    /**
     * @brief Register face and save register image
     * @param image: face image
     * @return register index. other return -1.
     */
    virtual qint64 Register(const QImage& image,
                            const QRect &face = QRect()) = 0;

    virtual int Delete(const qint64 &index) = 0;

    /**
     * @brief Query registered face
     * @param image: query face image
     * @return find index. other return -1
     */
    virtual qint64 Query(const QImage& image, const QRect &face = QRect()) = 0;

    /**
     * @brief Save feature to file
     * @param szFile: feature file name
     * @return 
     */
    virtual int Save(const QString &szFile = QString()) = 0;
    /**
     * @brief Load feature from file
     * @param szFile: feature file name
     * @return 
     */
    virtual int Load(const QString &szFile = QString()) = 0;

    /**
     * @brief GetRegisterImage
     * @param index: search register image index.
     *        when is -1, get register image path
     * @return register image file or path
     */
    virtual QString GetRegisterImage(qint64 index = -1);

    virtual bool IsValid();
    
    QString getImagePath();
    int setImagePath(const QString &szPath);

protected:
    CFace* m_pFace;
    bool m_bInit;
    QString m_szImagePath;
};

#endif // CRECOGNIZER_H_KL_2019_10_21_
