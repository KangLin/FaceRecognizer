#ifndef CFACETOOLSSEETA_H
#define CFACETOOLSSEETA_H

#include "FaceTools.h"
#include "seeta/QualityAssessor.h"

#include <QImage>
#include <QSharedPointer>

class CFaceToolsSeeta : public CFaceTools
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    explicit CFaceToolsSeeta(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual float EvaluateQuality(const QImage &image, const QRect &face);

private:
    virtual int UpdateParameter(QString &szErr);
    seeta::QualityAssessor m_QualityAssessor;
};

#endif // CFACETOOLSSEETA_H
