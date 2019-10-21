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

    virtual qint64 Register(const QImage &image,
                         const QVector<QPointF> &points);
    virtual qint64 Query(/*[in]*/ const QImage &image,
                      /*[in]*/ const QVector<QPointF> &points,
                      /*[out]*/ float *similarity = nullptr);
    virtual int Save(const QString &szFile);
    virtual int Load(const QString &szFile);
    virtual qint64 GetCount();
    
protected:
    virtual void UpdateParameter();
    QSharedPointer<seeta::FaceDatabase> m_Recognizer;
};

#endif // CRECOGNIZERSEETA_H
