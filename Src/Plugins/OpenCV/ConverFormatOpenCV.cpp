#include "ConverFormatOpenCV.h"
#include "Performance.h"

#if OpenCV_VERSION_MAJOR == 3 || OpenCV_VERSION_MAJOR == 2
#include "opencv/cv.hpp"
#else
#include "opencv2/opencv.hpp"
#endif

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(logOpenCV, "OpenCV")

CConverFormatOpenCV::CConverFormatOpenCV(QObject *parent) : CConverFormat(parent)
{}

QImage CConverFormatOpenCV::onConverFormatToRGB888(const QVideoFrame &frame)
{
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        qCritical(logOpenCV) << "videoFrame.map fail";
        return img;
    }
    PERFORMANCE(OpenCVConverFormatToRGB888)
    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
        cv::Mat out(videoFrame.height(), videoFrame.width(), CV_8UC3, img.bits());
        switch(videoFrame.pixelFormat())
        {
        case QVideoFrame::Format_YUV420P:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_I420);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_I420);
#endif
        }
            break;
        case QVideoFrame::Format_YV12:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_YV12);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_YV12);
#endif
        }
            break;
        case QVideoFrame::Format_NV21:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_NV21);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_NV21);
#endif
        }
            break;
        case QVideoFrame::Format_NV12:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_NV12);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_NV12);
#endif
        }
            break;
        case QVideoFrame::Format_YUYV:
        {
            //TODO： Test it
            cv::Mat in(videoFrame.height(),
                       videoFrame.width(), CV_8UC2, videoFrame.bits());
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_YUYV);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_YUYV);
#endif
        }
            break;
        case QVideoFrame::Format_UYVY:
        {
            //TODO： Test it
            cv::Mat in(videoFrame.height(),
                       videoFrame.width(), CV_8UC2, videoFrame.bits());
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_UYVY);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_UYVY);
#endif
        }
            break;


        default:
            qWarning(logOpenCV) << "OpenCVConverFormatToRGB888 Don't convert format:"
                                   << videoFrame.pixelFormat();
        }

    }while(0);
    videoFrame.unmap();
    PERFORMANCE_ADD_TIME(OpenCVConverFormatToRGB888, "OpenCVConverFormatToRGB888")
    return img;
}
