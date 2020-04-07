#include "DetectorSeeta.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>
#include <QDebug>

CDetectorSeeta::CDetectorSeeta(CFace *pFace, QObject *parent) 
    : CDetector(pFace, parent)
{
    m_bInit = false;
    m_MinFaceSize = 80;
}

CDetectorSeeta::~CDetectorSeeta()
{}

int CDetectorSeeta::Detect(const QImage &image,  QVector<QRect> &faces)
{
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    
    PERFORMANCE(SeetaDectect)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(SeetaDectect)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(SeetaDectect,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    img = img.rgbSwapped();
    PERFORMANCE_ADD_TIME(SeetaDectect, "rgbSwapped")
    SeetaImageData data;
    data.width = img.width();
    data.height = img.height();
    data.channels = 3;
    data.data = img.bits();
    
    faces.clear();
    PERFORMANCE_START(SeetaDectect)
    SeetaFaceInfoArray f = m_Dector->detect(data);
    PERFORMANCE_ADD_TIME(SeetaDectect, "detect:image width:"
                         + QString::number(image.width())
                         + ";Height:"
                         + QString::number(image.height()))
    for(int i = 0; i < f.size; i++)
    {
        QRect rect(f.data[i].pos.x,
                   f.data[i].pos.y,
                   f.data[i].pos.width,
                   f.data[i].pos.height);
        faces.push_back(rect);
    }
    PERFORMANCE_ADD_TIME(SeetaDectect, "copy reture value")
    return 0;
}

int CDetectorSeeta::UpdateParameter()
{
    //qDebug() << "CDetectorSeeta::UpdateParameter()";
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
        QString szErr =  "Don't support device " + QString::number(getDevice());
        LOG_MODEL_ERROR("CDetectorSeeta", szErr.toStdString().c_str());
        break;
    }
    
    int id = 0;
    
    QString szPath = getModelPath() + QDir::separator() + "Seeta";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    QString szFile = szPath + QDir::separator() + "fd_2_00.dat";
    try {
        seeta::ModelSetting model(szFile.toStdString(),
                                  device,
                                  id);
        m_Dector = QSharedPointer<seeta::FaceDetector>(new seeta::FaceDetector(model));
    } catch (...) {
        QString szErr = "Load model fail:" + szFile;
        LOG_MODEL_ERROR("CDetectorSeeta", szErr.toStdString().c_str());
        return -2;
    }
    m_Dector->set(seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, getMinFaceSize());
    m_bInit = true;
    return 0;
}

int CDetectorSeeta::getMinFaceSize()
{
    return m_MinFaceSize;
}

int CDetectorSeeta::setMinFaceSize(int size)
{
    m_MinFaceSize = size;
    m_Dector->set(seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, getMinFaceSize());
    return 0;
}
