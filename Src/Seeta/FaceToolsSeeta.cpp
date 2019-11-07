#include "FaceToolsSeeta.h"
#include "Log.h"
#include "Performance.h"

CFaceToolsSeeta::CFaceToolsSeeta(QObject *parent) : CFaceTools(parent)
{}

void CFaceToolsSeeta::UpdateParameter()
{
    if(!m_pParameter) return;

    m_QualityAssessor.setFaceSize(m_pParameter->GetMinFaceSize());
    return;
}

float CFaceToolsSeeta::EvaluateQuality(const QImage &image, const QRect &face, const QVector<QPointF> &points)
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
    std::vector<SeetaPointF> p;
    foreach (QPointF point, points) {
        SeetaPointF pp = {point.x(), point.y()};
        p.push_back(pp);
    }
    return m_QualityAssessor.evaluate(data, rect, p.data());
}
