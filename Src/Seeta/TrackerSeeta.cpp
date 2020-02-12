#include "TrackerSeeta.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

CTrackerSeeta::CTrackerSeeta(QObject *parent) : CTracker(parent)
{
    m_bInit = false;
}

int CTrackerSeeta::UpdateParameter(QString &szErr)
{
    if(!m_pParameter)
    {
        szErr = "The parameter is null";
        LOG_MODEL_ERROR("CTrackerSeeta", szErr.toStdString().c_str());
        return -1;
    }
    
    m_bInit = false;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    switch (m_pParameter->GetDevice()) {
    case CParameter::CPU:
        device = seeta::ModelSetting::CPU;
        break;
    case CParameter::GPU:
        device = seeta::ModelSetting::GPU;
        break;
    default:
        szErr = "Don't support device %d" +
                QString::number(m_pParameter->GetDevice());
        LOG_MODEL_ERROR("CDetectorSeeta", szErr.toStdString().c_str());
        break;
    }
    
    int id = 0;
    
    QString szFile = m_pParameter->GetModelPath() + QDir::separator() + "fd_2_00.dat";
    if(!QFile::exists(szFile))
    {
        szFile = m_pParameter->GetModelPath() + QDir::separator()
                + "Seeta" + QDir::separator() + "fd_2_00.dat";
    }
    try {
        seeta::ModelSetting model(szFile.toStdString(),
                                  device,
                                  id);
        m_Tracker = QSharedPointer<seeta::FaceTracker>(new seeta::FaceTracker(model));
    }catch(...){
        szErr = "Load model fail";
        LOG_MODEL_ERROR("CTrackerSeeta", szErr.toStdString().c_str());
        return -2;
    }
    m_Tracker->set(seeta::FaceTracker::PROPERTY_VIDEO_STABLE, 1);
    m_bInit = true;
    return 0;
}

int CTrackerSeeta::Track(const QImage &image, QVector<strFace> &faces)
{
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    
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
