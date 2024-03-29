#include "RecognizerOpenCV.h"

#include "Performance.h"
#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logOpenCV)
CRecognizerOpenCV::CRecognizerOpenCV(CFace *pFace, QObject *parent)
    : CRecognizer(pFace, parent)
{
    m_nMaxIndex = -1;
}

int CRecognizerOpenCV::UpdateParameter()
{
    int nRet = 0;
   
    m_bInit = false;
    
    QString szPath = getModelPath() + QDir::separator() + "Opencv";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    szPath = szPath + QDir::separator() + "haarcascades";
    qDebug(logOpenCV) << "The model files path:" << szPath;
    QString szFile = szPath + QDir::separator() + "haarcascade_frontalface_alt2.xml";
    try{
        m_Recognizer = cv::face::LBPHFaceRecognizer::create();
        if(!m_Recognizer)
        {
            qCritical(logOpenCV) << "Load model file fail";
            return -2;
        }
    } catch (cv::Exception e) {
        QString szErr = "Load model fail:";
        szErr += e.msg.c_str();
        qCritical(logOpenCV) << szErr;
        return -2;
    }

    m_bInit = true;

    return nRet;
}

qint64 CRecognizerOpenCV::Register(const QImage &image, const QRect &face)
{   
    PERFORMANCE(RecognizerOpenCV)
    QImage img = image;
    if(img.format() != QImage::Format_Grayscale8)
    {
        PERFORMANCE_START(RecognizerOpenCV)
        img = img.convertToFormat(QImage::Format_Grayscale8);
        PERFORMANCE_ADD_TIME(RecognizerOpenCV,
                             "convert format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }

	cv::Mat gray(img.height(), img.width(), CV_8UC1, img.bits());
    cv::Rect faceRect(face.x(), face.y(), face.width(), face.height());
    cv::Mat f = gray(faceRect);
    m_Recognizer->train(f, ++m_nMaxIndex);
    return m_nMaxIndex;
}

int CRecognizerOpenCV::Delete(const qint64 &index)
{
    int nRet = 0;
    
    return nRet;
}

qint64 CRecognizerOpenCV::Query(const QImage &image, const QRect &face)
{
    qint64 index = -1;
    
    return index;
}

int CRecognizerOpenCV::Save(const QString &szFile)
{
    int nRet = 0;
    
    return nRet;
}

int CRecognizerOpenCV::Load(const QString &szFile)
{
    int nRet = 0;
    
    return nRet;
}
