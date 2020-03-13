#include "DetectorOpenCV.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

CDetectorOpenCV::CDetectorOpenCV(CFace *pFace, QObject *parent)
    : CDetector(pFace, parent),
    m_bInit(false)
{}

int CDetectorOpenCV::UpdateParameter()
{
    int nRet = 0;
    
    m_bInit = false;
    
    QString szPath = getModelPath() + QDir::separator() + "Opencv";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    szPath = szPath + QDir::separator() + "haarcascades";
    LOG_MODEL_DEBUG("CDetectorOpenCV", "The model files path: %s",
                    szPath.toStdString().c_str());
    QString szFile = szPath + QDir::separator() + "haarcascade_frontalface_alt2.xml";
    try{
        m_haar_cascade = cv::Ptr<cv::CascadeClassifier>(new cv::CascadeClassifier());
        bool bRet = m_haar_cascade->load(szFile.toStdString());
        if(!bRet || m_haar_cascade->empty())
        {
            LOG_MODEL_ERROR("CDetectorOpenCV", "Load model file fail");
            return -2;
        }
    } catch (cv::Exception e) {
        QString szErr = "Load model fail:";
        szErr += e.msg.c_str();
        LOG_MODEL_ERROR("CDetectorOpenCV", szErr.toStdString().c_str());
        return -2;
    }

    m_bInit = true;

    return nRet;
}

int CDetectorOpenCV::Detect(const QImage &image, QVector<QRect> &faces)
{
    int nRet = 0;
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    if(m_haar_cascade->empty()) return -3;
    
    PERFORMANCE(OpencvDectect)
    QImage img = image;
    if(img.format() != QImage::Format_Grayscale8)
    {
        PERFORMANCE_START(OpencvDectect)
        img = img.convertToFormat(QImage::Format_Grayscale8);     
        PERFORMANCE_ADD_TIME(OpencvDectect,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }

	cv::Mat gray(img.height(), img.width(), CV_8UC1, img.bits());
    cv::equalizeHist(gray, gray);
    std::vector<cv::Rect> fs;
    PERFORMANCE_START(OpencvDectect)
    m_haar_cascade->detectMultiScale(gray, fs);
    PERFORMANCE_ADD_TIME(OpencvDectect, "detect:image width:"
                         + QString::number(image.width())
                         + ";Height:"
                         + QString::number(image.height()))
    foreach(auto f, fs)
    {
        QRect r(f.x, f.y, f.width, f.height);
        faces.push_back(r);
    }

    return nRet;
}
