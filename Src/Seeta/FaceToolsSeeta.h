#ifndef CFACETOOLSSEETA_H
#define CFACETOOLSSEETA_H

#include "FaceTools.h"
#include "seeta/QualityAssessor.h"

#include <QImage>
#include <QSharedPointer>

class CFaceToolsSeeta : public CFaceTools
{
    Q_OBJECT
public:
    explicit CFaceToolsSeeta(QObject *parent = nullptr);
    virtual float EvaluateQuality(const QImage &image,
                           const QRect &face,
                           const QVector<QPointF> &points);

private:
    virtual int UpdateParameter(QString &szErr);
    seeta::QualityAssessor m_QualityAssessor;
};

#endif // CFACETOOLSSEETA_H
