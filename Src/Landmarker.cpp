#include "Landmarker.h"
#include "Log.h"

CLandmarker::CLandmarker(CFace *pFace, QObject *parent)
    : CFaceBase(parent),
      m_pFace(pFace),
      m_nPoints(5)
{}

CLandmarker::~CLandmarker()
{}

int CLandmarker::Mark(const QImage &image,
                      const QVector<QRect> &faces,
                      QVector<QVector<QPointF> > &points)
{
    int nRet = 0;
    foreach(auto face, faces)
    {
        QVector<QPointF> p;
        nRet = Mark(image, face, p);
        if(nRet) return nRet;
        points.push_back(p);
    }
    return nRet;
}

int CLandmarker::setPoints(int nPoints)
{
    m_nPoints = nPoints;
    UpdateParameter();
    return 0;
}

int CLandmarker::getPoints()
{
    return m_nPoints;
}
