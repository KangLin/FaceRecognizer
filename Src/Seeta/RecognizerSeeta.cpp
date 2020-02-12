#include "RecognizerSeeta.h"
#include "Log.h"
#include "RabbitCommonDir.h"
#include "Performance.h"

#include <QDir>

CRecognizerSeeta::CRecognizerSeeta(QObject *parent)
    : CRecognizer(parent)
{
    m_bInit = false;
    m_fThreshold = 0.7f;
}

CRecognizerSeeta::~CRecognizerSeeta()
{}

int CRecognizerSeeta::UpdateParameter(QString &szErr)
{
    if(!m_pParameter)
    {
        szErr =  "The parameter is null";
        LOG_MODEL_ERROR("CDetectorSeeta", szErr.toStdString().c_str());
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
    
    try {
        QString szFile = m_pParameter->GetModelPath() + QDir::separator() + "fr_2_10.dat";
        if(!QFile::exists(szFile))
        {
            szFile = m_pParameter->GetModelPath() + QDir::separator()
                    + "Seeta" + QDir::separator() + "fr_2_10.dat";
        }
        seeta::ModelSetting model(szFile.toStdString(),
                                  device,
                                  id);
        m_Recognizer = QSharedPointer<seeta::FaceDatabase>(new seeta::FaceDatabase(model));
        if(!m_Recognizer)
        {
            szErr =  "new seeta::FaceDatabase fail";
            LOG_MODEL_ERROR("CRecognizerSeeta", szErr.toStdString().c_str());
            return -2;
        }        
    } catch (...) {
        szErr = "Load model fail";
        LOG_MODEL_ERROR("CRecognizerSeeta", szErr.toStdString().c_str());
        return -3;
    }

    m_bInit = true;
    
    Load();
}

qint64 CRecognizerSeeta::Register(const QImage &image,
                                  const QVector<QPointF> &points)
{
    int64_t index = 0;

    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    
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
