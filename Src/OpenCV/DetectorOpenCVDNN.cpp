#include "DetectorOpenCVDNN.h"
#include "Log.h"
#include "Performance.h"

#include <QDir>

// OpenCV4.0实现人脸识别: https://cloud.tencent.com/developer/article/1419671
//利用OpenCV和深度学习实现人脸检测:https://blog.csdn.net/amusi1994/article/details/79645403
//OpenCV3.3深度学习模块(DNN)应用-图像分类:https://blog.csdn.net/maweifei/article/details/78150233
// opencv基于DNN的人脸检测:    https://blog.csdn.net/qq_30815237/article/details/87914775
// OpenCV4.0 DNN-googleNet: https://blog.csdn.net/u011028771/article/details/84901361
// GPU编程--OpenCL基本概念: https://blog.csdn.net/w1992wishes/article/details/80426476
CDetectorOpenCVDNN::CDetectorOpenCVDNN(CFace *pFace, QObject *parent)
    : CDetector(pFace, parent),
      m_bInit(false)
{}

CDetectorOpenCVDNN::~CDetectorOpenCVDNN()
{}

int CDetectorOpenCVDNN::Detect(const QImage &image, QVector<QRect> &faces)
{
    int nRet = 0;
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    if(m_Net.empty()) return -3;
    
    PERFORMANCE(OpencvDnnDectect)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(OpencvDnnDectect)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(OpencvDnnDectect,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    // Set the size of image and meanval
    int inWidth = 300;
    int inHeight = 300;
    double inScaleFactor = 1.0;
    cv::Scalar meanVal(104.0, 177.0, 123.0);
    float confidenceThreshold = 0.7f; 
    
    // Load image
	cv::Mat frame(img.height(), img.width(), CV_8UC3, img.bits());
    
    PERFORMANCE_START(OpencvDnnDectect)
    // Prepare blob
    cv::Mat inputBlob = cv::dnn::blobFromImage(frame, inScaleFactor,
              cv::Size(inWidth, inHeight), meanVal, false, false);
    m_Net.setInput(inputBlob, "data");	// set the network input
    
    // Compute output，这是一个4维数，
    // rows and cols can only hold 2 dimensions,
    // so they are not used here, and set to -1
    cv::Mat detection = m_Net.forward("detection_out");
    
    //LOG_MODEL_DEBUG("CDetectorOpenCVDNN", m_Net.dump().c_str());
    
    //101*7矩阵
    cv::Mat detectionMat(detection.size[2], detection.size[3],
                         CV_32F, detection.ptr<float>());

    PERFORMANCE_ADD_TIME(OpencvDnnDectect, "detectionMat")

    for (int i = 0; i < detectionMat.rows; i++)  
    {  
        float confidence = detectionMat.at<float>(i, 2);  //第二列存放可信度
        
        if (confidence > confidenceThreshold)  
        {
            //存放人脸所在的图像中的位置信息
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

int CDetectorOpenCVDNN::UpdateParameter()
{
    int nRet = 0;
    m_bInit = false;
    
    QString szPath = getModelPath() + QDir::separator() + "Opencv";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    LOG_MODEL_DEBUG("CDetectorOpenCVDNN", "The model files path: %s",
                    szPath.toStdString().c_str());
    QString modelCaffDesc = szPath + QDir::separator() + "deploy.prototxt";
    QString modelCaffBinary = szPath + QDir::separator() + "res10_300x300_ssd_iter_140000_fp16.caffemodel";

    QString modelTFBinary = szPath + QDir::separator() + "opencv_face_detector_uint8.pb";
    QString modelTFDesc = szPath + QDir::separator() + "opencv_face_detector.pbtxt";
    try{
        //m_Net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        //m_Net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        
        //*
        if(QFile::exists(modelTFDesc) && QFile::exists(modelTFBinary))
            m_Net = cv::dnn::readNetFromTensorflow(modelTFBinary.toStdString(),
                                               modelTFDesc.toStdString());
        else //*/
            //*
            if(QFile::exists(modelCaffDesc) && QFile::exists(modelCaffBinary))
            m_Net = cv::dnn::readNetFromCaffe(modelCaffDesc.toStdString(),
                                          modelCaffBinary.toStdString());//*/
        if(m_Net.empty())
        {
            QString szErr = "Can't load network by using the following files: ";
            szErr = szErr + "prototxt:" + szPath;
            szErr = szErr + "caffemodel:" + szPath;
            szErr = szErr + "Models are available here:";
            szErr = szErr + "<OPENCV_SRC_DIR>/samples/dnn/face_detector";
            szErr = szErr + "or here:";
            szErr = szErr + "https://github.com/opencv/opencv/tree/master/samples/dnn/face_detector";
            LOG_MODEL_ERROR("CDetectorOpenCVDNN", szErr.toStdString().c_str());
            return -1;
        }
    } catch (cv::Exception e) {
        QString szErr = "Load model fail:";
        szErr += e.msg.c_str();
        LOG_MODEL_ERROR("CDetectorOpenCVDNN", szErr.toStdString().c_str());
        return -2;
    }

    m_bInit = true;
    return nRet;
}
