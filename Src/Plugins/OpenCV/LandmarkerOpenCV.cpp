#include "LandmarkerOpenCV.h"

#include "Performance.h"

#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logOpenCV)

CLandmarkerOpenCV::CLandmarkerOpenCV(CFace *pFace, QObject *parent)
    : CLandmarker(pFace, parent),
      m_bInit(false)
{}

int CLandmarkerOpenCV::UpdateParameter()
{
    int nRet = 0;
    m_bInit = false;

    QString szPath = getModelPath() + QDir::separator() + "Opencv";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    qDebug(logOpenCV) << "The model files path:" << szPath;
    QString szFile = szPath + QDir::separator() + "lbfmodel.yaml";

    try{
        m_Facemark = cv::face::FacemarkLBF::create();
        m_Facemark->loadModel(szFile.toStdString());
    } catch (cv::Exception e) {
        QString szErr = "Load model fail:";
        szErr += e.msg.c_str();
        qCritical(logOpenCV) << szErr;
        return -2;
    }

    m_bInit = true;
    return nRet;
}

int CLandmarkerOpenCV::Mark(const QImage &image, const QRect &face, QVector<QPointF> &points)
{
    int nRet = 0;
    if(!m_bInit) return -1;
    if(!m_Facemark) return -2;
    if(image.isNull()) return -1;

    PERFORMANCE(OpenCVMark)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(OpenCVMark)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(OpenCVMark,
                             "convert format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }

    // Load image
	cv::Mat frame(img.height(), img.width(), CV_8UC3, img.bits());

    std::vector<cv::Rect> faces;
    cv::Rect rect(face.x(), face.y(), face.width(), face.height());
    faces.push_back(rect);

    std::vector< std::vector<cv::Point2f> > landmarks;
    PERFORMANCE_START(OpenCVMark)
    bool success = m_Facemark->fit(frame, faces, landmarks);
    PERFORMANCE_ADD_TIME(OpenCVMark, "m_Facemark->fit")
    if(!success)
    {
        qCritical(logOpenCV) << "CLandmarkerOpenCV::Mark fail";
        return -3;
    }

    for(auto lm: landmarks)
        for(auto p: lm)
        {
            QPointF point(p.x, p.y);
            points.push_back(point);
        }

    return nRet;
}

int CLandmarkerOpenCV::Mark(const QImage &image,
                            const QVector<QRect> &faces,
                            QVector<QVector<QPointF> > &points)
{
    int nRet = 0;
    if(!m_bInit) return -1;
    if(!m_Facemark) return -2;
    if(image.isNull()) return -1;

    PERFORMANCE(OpenCVMark)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(OpenCVMark)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(OpenCVMark,
                             "convert format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }

    // Load image
	cv::Mat frame(img.height(), img.width(), CV_8UC3, img.bits());
    
    std::vector<cv::Rect> fs;
    foreach(auto face, faces)
    {
        cv::Rect rect(face.x(), face.y(), face.width(), face.height());
        fs.push_back(rect);        
    }
    
    std::vector< std::vector<cv::Point2f> > landmarks;
    bool success = m_Facemark->fit(frame, fs, landmarks);
    if(!success)
    {
        qCritical(logOpenCV) << "CLandmarkerOpenCV::Mark fail";
        return -3;
    }

    for(auto lm: landmarks)
    {
        QVector<QPointF> ps;
        for(auto p: lm)
        {
            QPointF point(p.x, p.y);
            ps.push_back(point);
        }
        points.push_back(ps);
    }
    return nRet;
}
