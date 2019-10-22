#ifndef CRECOGNIZER_H_KL_2019_10_21_
#define CRECOGNIZER_H_KL_2019_10_21_

#pragma once
#include <QObject>
#include <QImage>

#include "facerecognizer_export.h"
#include "ParameterRecognizer.h"

class FACERECOGNIZER_EXPORT CRecognizer : public QObject
{
    Q_OBJECT
public:
    CRecognizer(QObject* parent = nullptr);
    virtual ~CRecognizer();
    
    virtual int SetParameter(CParameterRecognizer *pPara);
    
    struct _INFO
    {
        qint64 index;
        qint64 no;
        QString szName;
        QString szImageFile;
    };
    virtual qint64 Register(const QImage &image,
                            const QVector<QPointF> &points,
                            /*[in/out]*/_INFO &info) = 0;
    
    virtual _INFO Query(/*[in]*/ const QImage &image,
                        /*[in]*/ const QVector<QPointF> &points) = 0;
    
    virtual int Save(const QString &szFile = QString()) = 0;
    virtual int Load(const QString &szFile = QString()) = 0;
    
    // Registed count
    virtual qint64 GetCount();
    
public Q_SLOTS:
    void slotParameterUpdate();
    void slotParameterDelete();

protected:
    virtual void UpdateParameter() = 0;
    virtual int SetCount(qint64 count);

    CParameterRecognizer* m_pParameter;
    qint64 m_nCount;
};

#endif // CRECOGNIZER_H_KL_2019_10_21_
