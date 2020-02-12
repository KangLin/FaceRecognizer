#include "DetectorOpenCV.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

// OpenCV4.0实现人脸识别: https://cloud.tencent.com/developer/article/1419671
//利用OpenCV和深度学习实现人脸检测:https://blog.csdn.net/amusi1994/article/details/79645403
//OpenCV3.3深度学习模块(DNN)应用-图像分类:https://blog.csdn.net/maweifei/article/details/78150233
// opencv基于DNN的人脸检测:    https://blog.csdn.net/qq_30815237/article/details/87914775
CDetectorOpenCV::CDetectorOpenCV(QObject *parent)
    : CDetector(parent),
      m_bInit(false)
{
}

CDetectorOpenCV::~CDetectorOpenCV()
{}

int CDetectorOpenCV::Detect(const QImage &image, QVector<QRect> &faces)
{
    int nRet = 0;
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    if(m_Net.empty()) return -3;
    
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(OpencvDectect)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(OpencvDectect,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    // Set the size of image and meanval
    int inWidth = 300;//img.width();
    int inHeight = 300;//img.height();
    double inScaleFactor = 1.0;
    cv::Scalar meanVal(104.0, 177.0, 123.0);
    float confidenceThreshold = 0.7f; 
    
    // Load image
	cv::Mat frame(img.width(), img.height(), CV_8UC3, img.bits());
    
    // Prepare blob
    cv::Mat inputBlob = cv::dnn::blobFromImage(frame, inScaleFactor,
              cv::Size(inWidth, inHeight), meanVal, false, false);
    m_Net.setInput(inputBlob, "data");	// set the network input
    cv::Mat detection = m_Net.forward("detection_out");	// compute output
    //LOG_MODEL_DEBUG("CDetectorOpenCV", m_Net.dump().c_str());
    cv::Mat detectionMat(detection.size[2], detection.size[3],
                         CV_32F, detection.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++)  
    {  
        float confidence = detectionMat.at<float>(i, 2);  
        
        if (confidence > confidenceThreshold)  
        {  
            int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);  
            int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);  
            int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);  
            int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);  
            
            QRect object((int)xLeftBottom, (int)yLeftBottom,  
                        (int)(xRightTop - xLeftBottom),  
                        (int)(yRightTop - yLeftBottom));  
            faces.push_back(object);
        }  
    }  
    
    return nRet;
}

int CDetectorOpenCV::UpdateParameter(QString &szErr)
{
    int nRet = 0;
    
    if(!m_pParameter)
    {
        szErr = "The parameter is null";
        LOG_MODEL_ERROR("CDetectorSeeta", szErr.toStdString().c_str());
        return -1;
    }
    
    m_bInit = false;
    
    QString szPath = "C:\\Users\\k\\Downloads\\opencv_dnn_model"; //m_pParameter->GetModelPath();
    QString modelCaffDesc = szPath + QDir::separator() + "deploy.prototxt";
    QString modelCaffBinary = szPath + QDir::separator() + "res10_300x300_ssd_iter_140000_fp16.caffemodel";

    QString modelTFBinary = szPath + QDir::separator() + "opencv_face_detector_uint8.pb";
    QString modelTFDesc = szPath + QDir::separator() + "opencv_face_detector.pbtxt";
    try{
        m_Net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        m_Net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        
        //*
        m_Net = cv::dnn::readNetFromTensorflow(modelTFBinary.toStdString(),
                                               modelTFDesc.toStdString());//*/
        
        /*
        m_Net = cv::dnn::readNetFromCaffe(modelCaffDesc.toStdString(),
                                          modelCaffBinary.toStdString());//*/
        if(m_Net.empty())
        {
            szErr = "Can't load network by using the following files: ";
            szErr = szErr + "prototxt:" + szPath;
            szErr = szErr + "caffemodel:" + szPath;
            szErr = szErr + "Models are available here:";
            szErr = szErr + "<OPENCV_SRC_DIR>/samples/dnn/face_detector";
            szErr = szErr + "or here:";
            szErr = szErr + "https://github.com/opencv/opencv/tree/master/samples/dnn/face_detector";
            LOG_MODEL_ERROR("CDetectorOpenCV", szErr.toStdString().c_str());
            return -1;
        }
    } catch (...) {
        szErr = "Load model fail";
        LOG_MODEL_ERROR("CDetectorOpenCV", szErr.toStdString().c_str());
        return -2;
    }

    m_bInit = true;
    return nRet;
}
