#include "RecognizerSeeta.h"
#include "Log.h"
#include "RabbitCommonDir.h"
#include "Performance.h"
#include "FaceSeeta2.h"

#include <QDir>

CRecognizerSeeta::CRecognizerSeeta(CFace *pFace, QObject *parent)
    : CRecognizer(pFace, parent)
{
    m_fThreshold = 0.7f;
    m_szFeatureFile = RabbitCommon::CDir::Instance()->GetDirUserData()
            + QDir::separator()
            + "seeta_feature.dat";
}

CRecognizerSeeta::~CRecognizerSeeta()
{}

int CRecognizerSeeta::UpdateParameter()
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
        QString szErr = "Don't support device %d" +
                QString::number(getDevice());
        LOG_MODEL_ERROR("CDetectorSeeta", szErr.toStdString().c_str());
        break;
    }
    
    int id = 0;

    QString szPath = getModelPath() + QDir::separator() + "Seeta";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    QString szFile = szPath + QDir::separator() + "fr_2_10.dat";
    try {     
        seeta::ModelSetting model(szFile.toStdString(),
                                  device,
                                  id);
        m_Recognizer = QSharedPointer<seeta::FaceDatabase>(new seeta::FaceDatabase(model));
        if(!m_Recognizer)
        {
            QString szErr =  "new seeta::FaceDatabase fail";
            LOG_MODEL_ERROR("CRecognizerSeeta", szErr.toStdString().c_str());
            return -2;
        }        
    } catch (...) {
        QString szErr = "Load model fail：" + szFile;
        LOG_MODEL_ERROR("CRecognizerSeeta", szErr.toStdString().c_str());
        return -3;
    }

    m_bInit = true;
    
    return Load();
}

qint64 CRecognizerSeeta::Register(const QImage &image, const QRect &face)
{
    if(image.isNull() || !m_bInit || !m_pFace)
        return -1;
    
    QVector<QPointF> points;
    if(m_pFace->GetLandmarker()->Mark(image, face, points))
    {
        LOG_MODEL_ERROR("RecognizerSeeta", "GetLandmarker()->Mark fail");
        return -1;
    }
    
    return Register(image, points);
}

qint64 CRecognizerSeeta::Register(const QImage &image,
                                  const QVector<QPointF> &points)
{
    int64_t index = -1;

    if(image.isNull() || !m_bInit) return -1;
    
    PERFORMANCE(SeetaRegister)
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

qint64 CRecognizerSeeta::Query(const QImage &image, const QRect &face)
{
    qint64 index = -1;
    if(image.isNull() || !m_bInit) return -1;

    QVector<QPointF> points;
    if(m_pFace->GetLandmarker()->Mark(image, face, points))
    {
        LOG_MODEL_ERROR("RecognizerSeeta", "GetLandmarker()->Mark fail");
        return -1;
    }
    
    index = Query(image, points);
    
    return index;
}

qint64 CRecognizerSeeta::Query(/*[in]*/ const QImage &image,
                               /*[in]*/ const QVector<QPointF> &points)
{
    if(image.isNull() || !m_bInit) return -1;
    
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
        file = getFeatureFile();
    if(m_Recognizer->Save(file.toStdString().c_str()))
        return 0;
    return -1;
}

int CRecognizerSeeta::Load(const QString &szFile)
{
    QString file = szFile;
    if(szFile.isEmpty())
        file = getFeatureFile();
    if(m_Recognizer->Load(file.toStdString().c_str()))
        return 0;
    return -1;
}

float CRecognizerSeeta::getThreshold()
{
    return m_fThreshold;
}

int CRecognizerSeeta::setThreshold(float threshold)
{
    m_fThreshold = threshold;
    return 0;
}

QString CRecognizerSeeta::getFeatureFile()
{
    return m_szFeatureFile;
}

int CRecognizerSeeta::setFeatureFile(const QString &szFile)
{
    m_szFeatureFile = szFile;
    return 0;
}
