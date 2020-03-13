#include "LandmarkerSeeta.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

CLandmarkerSeeta::CLandmarkerSeeta(CFace *pFace, QObject *parent)
    : CLandmarker(pFace, parent)
{
    m_bInit = false;
}

CLandmarkerSeeta::~CLandmarkerSeeta()
{}

int CLandmarkerSeeta::UpdateParameter()
{
    m_bInit = false;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    switch (getDevice()) {
    case CPU:
        device = seeta::ModelSetting::CPU;
        break;
    case GPU:
        device = seeta::ModelSetting::GPU;
        break;
    default:
        QString szErr = "Don't support device" +
                QString::number(getDevice());
        LOG_MODEL_ERROR("CLandmarkerSeeta", szErr.toStdString().c_str());
        break;
    }
    
    int id = 0;
    
    QString szPath = getModelPath() + QDir::separator() + "Seeta";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    QString szFile = szPath + QDir::separator();
    if(getPoints() == 81)
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
        QString szErr = "Load model fail:" + szFile;
        LOG_MODEL_ERROR("CLandmarkerSeeta", szErr.toStdString().c_str());
        return -1;
    }
    m_bInit = true;
    return 0;
}

int CLandmarkerSeeta::Mark(const QImage &image, const QRect &face, QVector<QPointF> &points)
{
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;

    PERFORMANCE(SeetaMark)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(SeetaMark)
        img = img.convertToFormat(QImage::Format_RGB888);
        PERFORMANCE_ADD_TIME(SeetaMark, "convertToFormat")
    }
    img = img.rgbSwapped();
    PERFORMANCE_ADD_TIME(SeetaMark, "rgbSwapped")

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
    PERFORMANCE_START(SeetaMark)
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
