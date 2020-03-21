#ifndef CRECOGNIZERSEETA_H
#define CRECOGNIZERSEETA_H

#include "Recognizer.h"
#include "seeta/FaceDatabase.h"

#include <QSharedPointer>

class CRecognizerSeeta : public CRecognizer
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

    Q_PROPERTY(float threshold READ getThreshold WRITE setThreshold)
    Q_PROPERTY(QString featureFile READ getFeatureFile WRITE setFeatureFile)
    
public:
    CRecognizerSeeta(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual ~CRecognizerSeeta();

    /**
     * @brief Register face and save register image
     * @param image: face image
     * @return register index. other return -1.
     */
    virtual qint64 Register(const QImage& image, const QRect &face = QRect());
    
    virtual int Delete(const qint64 &index);
    /**
     * @brief Query register face
     * @param image: query face image
     * @return find index. other return -1
     */
    virtual qint64 Query(const QImage& image, const QRect &face = QRect());

    /**
     * @brief Save feature to file
     * @param szFile: feature file name
     * @return 
     */
    virtual int Save(const QString &szFile = QString());
    /**
     * @brief Load feature from file
     * @param szFile: feature file name
     * @return 
     */
    virtual int Load(const QString &szFile = QString());
    
    float getThreshold();
    int setThreshold(float threshold);
    
    QString getFeatureFile();
    int setFeatureFile(const QString &szFile);

protected:
    virtual int UpdateParameter();
    QSharedPointer<seeta::FaceDatabase> m_Recognizer;
    float m_fThreshold;
    QString m_szFeatureFile;

    /**
     * @brief Register face and save register image
     * @param image: face image
     * @param points: feature points
     * @return register index. other return -1.
     */
    virtual qint64 Register(const QImage &image,
                            const QVector<QPointF> &points);
    /**
     * @brief Query register face
     * @param image: query face image
     * @param points: feature points
     * @return find index. other return -1
     */
    virtual qint64 Query(/*[in]*/ const QImage &image,
                         /*[in]*/ const QVector<QPointF> &points);
};

#endif // CRECOGNIZERSEETA_H
