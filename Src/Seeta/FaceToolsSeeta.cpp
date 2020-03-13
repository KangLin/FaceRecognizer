#include "FaceToolsSeeta.h"
#include "Log.h"
#include "Performance.h"
#include "Face.h"

CFaceToolsSeeta::CFaceToolsSeeta(CFace *pFace, QObject *parent)
    : CFaceTools(pFace, parent)
{
    m_MinFaceSize = 80;
}

int CFaceToolsSeeta::getMinFaceSize()
{
    return m_MinFaceSize;
}

int CFaceToolsSeeta::setMinFaceSize(int size)
{
    m_MinFaceSize = size;
    UpdateParameter();
    return 0;
}

int CFaceToolsSeeta::UpdateParameter()
{
    m_QualityAssessor.setFaceSize(getMinFaceSize());
    return 0;
}

float CFaceToolsSeeta::EvaluateQuality(const QImage &image, const QRect &face)
{
    if(image.isNull()) return -1;
    PERFORMANCE(CFaceToolsSeeta)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(CFaceToolsSeeta)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(CFaceToolsSeeta,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    img = img.rgbSwapped();
    PERFORMANCE_ADD_TIME(CFaceToolsSeeta, "rgbSwapped")
    SeetaImageData data;
    data.width = img.width();
    data.height = img.height();
    data.channels = 3;
    data.data = img.bits();
    
    SeetaRect rect = {face.x(), face.y(), face.width(), face.height()};
    
    QVector<QPointF> points;
    int nRet = m_pFace->GetLandmarker()->Mark(image, face, points);
    if(nRet) return 0;
    std::vector<SeetaPointF> p;
    foreach (QPointF point, points) {
        SeetaPointF pp = {point.x(), point.y()};
        p.push_back(pp);
    }
    return m_QualityAssessor.evaluate(data, rect, p.data());
}
