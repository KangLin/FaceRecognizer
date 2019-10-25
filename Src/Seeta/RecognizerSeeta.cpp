#include "RecognizerSeeta.h"
#include "Log.h"
#include "RabbitCommonDir.h"
#include "Performance.h"

#include <QDir>

CRecognizerSeeta::CRecognizerSeeta(QObject *parent)
    : CRecognizer(parent)
{
    m_fThreshold = 0.7f;
}

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
    
    Load();
}

qint64 CRecognizerSeeta::Register(const QImage &image,
                                  const QVector<QPointF> &points)
{
    int64_t index = 0;

    PERFORMANCE(SeetaRegister);
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

    std::vector<SeetaPointF> p;
    foreach (QPointF point, points) {
        SeetaPointF pp = {point.x(), point.y()};
        p.push_back(pp);
    }

    PERFORMANCE_START(SeetaRegister)
    index = m_Recognizer->Register(data, p.data());
    PERFORMANCE_ADD_TIME(SeetaRegister, "Register")
    if(!img.rgbSwapped().save(GetRegisterImage(index)))
        LOG_MODEL_ERROR("CRecognizerSeeta", "Save register image fail");
    
    return index;
}

int CRecognizerSeeta::Delete(const qint64 &index)
{
    int nRet = m_Recognizer->Delete(index);
    if(1 == nRet)
    {
        QDir d;
        d.remove(GetRegisterImage(index));
        return 0;
    }
    return -1;
}
qint64 CRecognizerSeeta::Query(/*[in]*/ const QImage &image,
                                /*[in]*/ const QVector<QPointF> &points)
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

    std::vector<SeetaPointF> p;
    foreach (QPointF point, points) {
        SeetaPointF pp = {point.x(), point.y()};
        p.push_back(pp);
    }

    float similarity = 0;
    qint64 index = m_Recognizer->Query(data, p.data(), &similarity);
    if(m_fThreshold > similarity)
        index = -1;

    return index;
}

int CRecognizerSeeta::Save(const QString &szFile)
{
    QString file = szFile;
    if(szFile.isEmpty())
        file = m_pParameter->GetFeatureFile();
    if(m_Recognizer->Save(file.toStdString().c_str()))
        return 0;
    return -1;
}

int CRecognizerSeeta::Load(const QString &szFile)
{
    QString file = szFile;
    if(szFile.isEmpty())
        file = m_pParameter->GetFeatureFile();
    if(m_Recognizer->Load(file.toStdString().c_str()))
    {
        return 0;
    }
    return -1;
}
