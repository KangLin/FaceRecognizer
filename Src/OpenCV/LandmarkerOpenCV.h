#ifndef LANDMARKEROPENCV_H
#define LANDMARKEROPENCV_H

/**
  * @diafile  利用OpenCV的LBF算法进行人脸关键点检测
  * @author  Kang Lin(kl222@126.com)
  * @ref [1]Tutorial: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/
  * @ref [2]Code: https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection
  * @note OpenCV3.4以及上支持Facemark
  *
  */

//https://github.com/amusi/opencv-facial-landmark-detection

#include <opencv2/opencv.hpp>

#ifdef HAVE_OPENCV_FACE
#include <opencv2/face.hpp>
#endif

#include "Landmarker.h"

class CLandmarkerOpenCV : public CLandmarker
{
    Q_OBJECT
public:
    explicit CLandmarkerOpenCV(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual int Mark(const QImage &image,
                     const QRect &face,
                     QVector<QPointF> &points);
    virtual int Mark(const QImage &image,
                     const QVector<QRect> &faces,
                     QVector<QVector<QPointF> > &points);
private:
    virtual int UpdateParameter();
    bool m_bInit;
    
    cv::Ptr<cv::face::Facemark> m_Facemark;
};

#endif // LANDMARKEROPENCV_H
