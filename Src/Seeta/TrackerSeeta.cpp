#include "TrackerSeeta.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

CTrackerSeeta::CTrackerSeeta(QObject *parent) : CTracker(parent)
{
}

void CTrackerSeeta::UpdateParameter()
{
    if(!m_pParameter)
    {
        LOG_MODEL_ERROR("CTrackerSeeta", "The parameter is null");
        return;
    }
    
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    switch (m_pParameter->GetDevice()) {
    case CParameter::CPU:
        device = seeta::ModelSetting::CPU;
        break;
    case CParameter::GPU:
        device = seeta::ModelSetting::GPU;
        break;
    default:
        LOG_MODEL_ERROR("CDetectorSeeta", "Don't support device %d",
                        m_pParameter->GetDevice());
        break;
    }
    
    int id = 0;
    
    QString szFile = m_pParameter->GetModelPath() + QDir::separator() + "fd_2_00.dat";
    try {
        seeta::ModelSetting model(szFile.toStdString(),
                                  device,
                                  id);
        m_Tracker = QSharedPointer<seeta::FaceTracker>(new seeta::FaceTracker(model));
    }catch(...){
        LOG_MODEL_ERROR("CTrackerSeeta", "Load model fail");
        return;
    }
    m_Tracker->set(seeta::FaceTracker::PROPERTY_VIDEO_STABLE, 1);
    return;
}

int CTrackerSeeta::Track(const QImage &image, QVector<strFace> &faces)
{
    if(image.isNull()) return -1;
    PERFORMANCE(CTrackerSeeta)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(CTrackerSeeta)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(CTrackerSeeta,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    img = img.rgbSwapped();
    PERFORMANCE_ADD_TIME(CTrackerSeeta, "rgbSwapped")
    SeetaImageData data;
    data.width = img.width();
    data.height = img.height();
    data.channels = 3;
    data.data = img.bits();
    
    auto f = m_Tracker->track(data);
    PERFORMANCE_ADD_TIME(CTrackerSeeta, "track")

    if(f.size <= 0) return -2;
    for (int i = 0; i < f.size; ++i) {
        strFace face;
        face.pid = f.data[i].PID;
        face.rect.setX(f.data[i].pos.x);
        face.rect.setY(f.data[i].pos.y);
        face.rect.setWidth(f.data[i].pos.width);
        face.rect.setHeight(f.data[i].pos.height);
        faces.push_back(face);
    }
    return 0;
}
