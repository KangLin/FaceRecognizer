#include "RecognizerSeeta.h"
#include "Log.h"
#include <QDir>

CRecognizerSeeta::CRecognizerSeeta(QObject *parent)
    : CRecognizer(parent)
{}

CRecognizerSeeta::~CRecognizerSeeta()
{}

void CRecognizerSeeta::UpdateParameter()
{
    if(!m_pParameter)
    {
        LOG_MODEL_ERROR("CDetectorSeeta", "The parameter is null");
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
    
    QString szFile = m_pParameter->GetModelPath() + QDir::separator() + "fr_2_10.dat";
    seeta::ModelSetting model(szFile.toStdString(),
                              device,
                              id);
    m_Recognizer = QSharedPointer<seeta::FaceDatabase>(new seeta::FaceDatabase(model));
    if(!m_Recognizer)
    {
        LOG_MODEL_ERROR("CRecognizerSeeta", "new seeta::FaceDatabase fail");
        return;
    }
    
    Load(m_pParameter->GetFeatureFile());
}

qint64 CRecognizerSeeta::Register(const QImage &image, const QVector<QPointF> &points)
{
    int64_t index = 0;

    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        img = img.convertToFormat(QImage::Format_RGB888).rgbSwapped();
    }
    
    SeetaImageData data;
    data.width = img.width();
    data.height = img.height();
    data.channels = 3;
    data.data = img.bits();

    std::vector<SeetaPointF> p;
    foreach (QPointF point, points) {
        SeetaPointF pp = {point.x(), point.y()};
        p.push_back(pp);
    }

    index = m_Recognizer->Register(data, p.data());
    
    return index;
}

qint64 CRecognizerSeeta::Query(const QImage &image,
                               const QVector<QPointF> &points,
                               float *similarity)
{
    int64_t index = 0;

    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        img = img.convertToFormat(QImage::Format_RGB888).rgbSwapped();
    }
    
    SeetaImageData data;
    data.width = img.width();
    data.height = img.height();
    data.channels = 3;
    data.data = img.bits();

    std::vector<SeetaPointF> p;
    foreach (QPointF point, points) {
        SeetaPointF pp = {point.x(), point.y()};
        p.push_back(pp);
    }

    index = m_Recognizer->Query(data, p.data(), similarity);
    
    return index;
}

int CRecognizerSeeta::Save(const QString &szFile)
{
    if(m_Recognizer->Save(szFile.toStdString().c_str()))
        return 0;
    return -1;
}

int CRecognizerSeeta::Load(const QString &szFile)
{
    if(m_Recognizer->Load(szFile.toStdString().c_str()))
    {
        SetCount(m_Recognizer->Count());
        return 0;
    }
    return -1;
}

qint64 CRecognizerSeeta::GetCount()
{
    return m_Recognizer->Count();
}
