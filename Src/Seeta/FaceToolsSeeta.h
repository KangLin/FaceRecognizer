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
    Q_PROPERTY(int minFaceSize READ getMinFaceSize WRITE setMinFaceSize)
    
public:
    explicit CFaceToolsSeeta(CFace* pFace = nullptr, QObject *parent = nullptr);
    virtual float EvaluateQuality(const QImage &image, const QRect &face);
    
    int getMinFaceSize();
    int setMinFaceSize(int size);
    
private:
    virtual int UpdateParameter();
    seeta::QualityAssessor m_QualityAssessor;
    int m_MinFaceSize;
};

#endif // CFACETOOLSSEETA_H
