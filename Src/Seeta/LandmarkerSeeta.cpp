#include "LandmarkerSeeta.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

CLandmarkerSeeta::CLandmarkerSeeta(QObject *parent)
    : CLandmarker(parent)
{}

CLandmarkerSeeta::~CLandmarkerSeeta()
{
}

void CLandmarkerSeeta::UpdateParameter()
{
    if(!m_pParameter)
        return;
    
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    switch (m_pParameter->GetDevice()) {
    case CParameter::CPU:
        device = seeta::ModelSetting::CPU;
        break;
    case CParameter::GPU:
        device = seeta::ModelSetting::GPU;
        break;
    default:
        LOG_MODEL_ERROR("CLandmarkerSeeta", "Don't support device %d",
                        m_pParameter->GetDevice());
        break;
    }
    
    int id = 0;
    
    QString szFile = m_pParameter->GetModelPath() + QDir::separator();
    if(m_pParameter->GetPoints() == 81)
        szFile += "pd_2_00_pts81.dat";
    else 
        szFile += "pd_2_00_pts5.dat";
    
    try {
        seeta::ModelSetting model(szFile.toStdString(),
                                  device,
                                  id);
        m_Landmarker = QSharedPointer<seeta::FaceLandmarker>(
                              new seeta::FaceLandmarker(model));   
    } catch (...) {
        LOG_MODEL_ERROR("CLandmarkerSeeta", "Load model fail");
        return;
    }
    return;
}

int CLandmarkerSeeta::Mark(const QImage &image, const QRect &face, QVector<QPointF> &points)
{
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        img = img.convertToFormat(QImage::Format_RGB888);
    }
    img = img.rgbSwapped();
    
    SeetaImageData data;
    data.width = img.width();
    data.height = img.height();
    data.channels = 3;
    data.data = img.bits();
    
    SeetaRect rect;
    rect.x = face.x();
    rect.y = face.y();
    rect.width = face.width();
    rect.height = face.height();
    
    points.clear();
    PERFORMANCE(SeetaMark)
    std::vector<SeetaPointF> p = m_Landmarker->mark(data, rect);
    PERFORMANCE_ADD_TIME(SeetaMark, "mark")
    std::vector<SeetaPointF>::iterator it;
    for(it = p.begin(); it != p.end(); it++)
    {
        QPointF p(it->x, it->y);
        points.push_back(p);
    }
    return 0;
}
