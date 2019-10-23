#ifndef CRECOGNIZERSEETA_H
#define CRECOGNIZERSEETA_H

#include "Recognizer.h"
#include "seeta/FaceDatabase.h"

#include <QSharedPointer>

class CRecognizerSeeta : public CRecognizer
{
    Q_OBJECT
    
public:
    CRecognizerSeeta(QObject *parent = nullptr);
    virtual ~CRecognizerSeeta();

    /**
     * @brief Register face and save register image
     * @param image: face image
     * @param points: feature points
     * @return register index. other return -1.
     */
    virtual qint64 Register(const QImage &image,
                            const QVector<QPointF> &points);
    virtual int Delete(const qint64 &index);
    /**
     * @brief Query register face
     * @param image: query face image
     * @param points: feature points
     * @return find index. other return -1
     */
    virtual qint64 Query(/*[in]*/ const QImage &image,
                        /*[in]*/ const QVector<QPointF> &points);
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
    
protected:
    virtual void UpdateParameter();
    QSharedPointer<seeta::FaceDatabase> m_Recognizer;
    float m_fThreshold;
};

#endif // CRECOGNIZERSEETA_H
