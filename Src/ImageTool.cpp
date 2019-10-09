#include "ImageTool.h"
#include "Log.h"
#include "yuv2rgb/yuv2rgb.h"

#ifdef HAVE_OPENCV
    #if OpenCV_VERSION_MAJOR == 3 || OpenCV_VERSION_MAJOR == 2
        #include "opencv/cv.hpp"
    #else
        #include "opencv2/opencv.hpp"
    #endif
#elif HAVE_LIBYUV
    #include "libyuv.h"
#endif

#include <QFileInfo>
#include <QDir>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <sstream>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QPainter>

CImageTool::CImageTool(QObject *parent) :
    QObject(parent)
{
}

CImageTool::~CImageTool()
{
}

//设置日志的回调函数  
void Log(void*, int, const char* fmt, va_list vl)
{
    LOG_MODEL_DEBUG("ffmpeg", fmt, vl);
}

#ifdef HAVE_FFMPEG
int CImageTool::SetFFmpegLog()
{
    //在程序初始化时设置ffmpeg日志的回调函数  
    av_log_set_callback(Log);
    return 0;
}

AVPixelFormat CImageTool::QVideoFrameFormatToFFMpegPixFormat(
        const QVideoFrame::PixelFormat format)
{
    switch (format) {
    case QVideoFrame::Format_RGB32:
        return AV_PIX_FMT_RGB32;
    case QVideoFrame::Format_RGB24:
        return AV_PIX_FMT_RGB24;
    case QVideoFrame::Format_YUYV:
        return AV_PIX_FMT_YUYV422;
    case QVideoFrame::Format_UYVY:
        return AV_PIX_FMT_UYVY422;
    case QVideoFrame::Format_NV21:
        return AV_PIX_FMT_NV21;
    case QVideoFrame::Format_NV12:
        return AV_PIX_FMT_NV12; 
    case QVideoFrame::Format_YUV420P:
        return AV_PIX_FMT_YUV420P;
    case QVideoFrame::Format_YV12:
    default:
        LOG_MODEL_ERROR("CImageTool",  "Don't conver format: %d", format);
        return AV_PIX_FMT_NONE;
    }
}

AVPixelFormat CImageTool::QImageFormatToFFMpegPixFormat(const QImage::Format format)
{
    if(QImage::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    return AV_PIX_FMT_NONE;
}

//如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存  
//成功返回0，不成功返回非0  
int CImageTool::ConvertFormat(/*[in]*/ const AVPicture &inFrame,
                         /*[in]*/ int nInWidth,
                         /*[in]*/ int nInHeight,
                         /*[in]*/ AVPixelFormat inPixelFormat,
                         /*[out]*/AVPicture &outFrame,
                         /*[in]*/ int nOutWidth,
                         /*[in]*/ int nOutHeight,
                         /*[in]*/ AVPixelFormat outPixelFormat)
{
    int nRet = 0;
    struct SwsContext* pSwsCtx = NULL;
    
    //分配输出空间  
    /*nRet = avpicture_alloc(&outFrame, outPixelFormat, nOutWidth, nOutHeight);
    if(nRet)
    {
        LOG_MODEL_ERROR("Tool", "avpicture_alloc fail:%x", nRet);
        return nRet;
    }*/
    
    if(inPixelFormat == outPixelFormat
            && nInWidth == nOutWidth
            && nInHeight == nOutHeight)
    {
        av_picture_copy(&outFrame, &inFrame, inPixelFormat,
                        nInWidth, nInHeight);
        return 0;
    }
    
    //设置图像转换上下文  
    pSwsCtx = sws_getCachedContext (NULL,
                                    nInWidth,                //源宽度  
                                    nInHeight,               //源高度  
                                    inPixelFormat,           //源格式  
                                    nOutWidth,               //目标宽度  
                                    nOutHeight,              //目标高度  
                                    outPixelFormat,          //目的格式  
                                    SWS_FAST_BILINEAR,       //转换算法  
                                    NULL, NULL, NULL);
    if(NULL == pSwsCtx)
    {
        LOG_MODEL_ERROR("Tool", "sws_getContext false");
        return -3;
    }
    
    //进行图片转换  
    nRet = sws_scale(pSwsCtx,
                     inFrame.data, inFrame.linesize,
                     0, nInHeight,
                     outFrame.data, outFrame.linesize);
    if(nRet < 0)
    {
        LOG_MODEL_ERROR("Tool", "sws_scale fail:%x", nRet);
    }
    else
    {
        nRet = 0;
    }
    
    sws_freeContext(pSwsCtx);
    return nRet;
}

#endif

// 图像格式介绍：
//    https://blog.csdn.net/byhook/article/details/84037338
//    https://blog.csdn.net/cgwang_1580/article/details/79595958
QImage CImageTool::ConverFormatToRGB888(const QVideoFrame &frame)
{
    switch(frame.pixelFormat())
    {
    case QVideoFrame::Format_YUV420P:
    case QVideoFrame::Format_NV21:
    case QVideoFrame::Format_NV12:
    case QVideoFrame::Format_YV12:
    case QVideoFrame::Format_YUYV:
    case QVideoFrame::Format_UYVY:
#if HAVE_OPENCV
        return OpenCVConverFormatToRGB888(frame);
#elif HAVE_FFMPEG
        return FFMpegConverFormatToRGB888(frame);
#elif HAVE_LIBYUV
        return  LibyuvConverFormatToRGB888(frame);
#else
        if(QVideoFrame::Format_YUV420P != frame.pixelFormat())
        {
            LOG_MODEL_WARNING("CImageTool", "Please use one of opencv, ffmpeg, libyuv");
        }
        break;
#endif
    default:
        break;
    }

    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;
    do{
        QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(
                    videoFrame.pixelFormat());
        if(QImage::Format_Invalid != f)
        {
            img = QImage(videoFrame.bits(),
                         videoFrame.width(),
                         videoFrame.height(),
                         videoFrame.width() << 2,
                         f);
        } else {
            switch(videoFrame.pixelFormat())
            {
            case QVideoFrame::Format_Jpeg:
                {
                    img.loadFromData(videoFrame.bits(),
                                     videoFrame.mappedBytes(),
                                     "JPEG");
                }
                break;
            case QVideoFrame::Format_YUV420P:
                YUV420_2_RGB(videoFrame.bits(), img.bits(),
                             videoFrame.width(), videoFrame.height());
                break;
            default:
                LOG_MODEL_ERROR("CImageTool",  "Don't implement conver format: %d",
                                videoFrame.pixelFormat());
            }
        }
    }while(0);
    videoFrame.unmap();
    return img;
}

#if HAVE_OPENCV

QImage CImageTool::OpenCVConverFormatToRGB888(const QVideoFrame &frame)
{
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("CImageTool", "videoFrame.map fail");
        return img;
    }
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
#if OpenCV_VERSION_MAJOR == 4
            cv::cvtColor(in, out, cv::COLOR_YUV420p2RGB);
#elif OpenCV_VERSION_MAJOR == 3
            cv::cvtColor(in, out, CV_YUV420p2RGB);
#else
            
#endif
            img = img.rgbSwapped();
        }
            break;
        case QVideoFrame::Format_NV21:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR == 4
            cv::cvtColor(in, out, cv::COLOR_YUV420sp2RGB);
#elif OpenCV_VERSION_MAJOR == 3 
            cv::cvtColor(in, out, CV_YUV420sp2RGB);
#else
   
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
        case QVideoFrame::Format_YV12:
        
        default:
            LOG_MODEL_WARNING("CImageTool",  "OpenCVConverFormatToRGB888 Don't conver format: %d",
                            videoFrame.pixelFormat());
        }
        
    }while(0);
    videoFrame.unmap();
    return img;

}

#elif HAVE_FFMPEG

QImage CImageTool::FFMpegConverFormatToRGB888(const QVideoFrame &frame)
{
    int nRet = 0;
    AVPicture inPic, outPic;
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;

    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
        nRet = avpicture_fill(&outPic, img.bits(),
                              AV_PIX_FMT_RGB24,
                              img.width(),
                              img.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CImageTool", "avpicture_get_size fail:%d", nRet);
            break;
        }
        nRet = avpicture_fill(&inPic, videoFrame.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                              videoFrame.width(),
                              videoFrame.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CImageTool", "avpicture_fill is fail");
            break;
        }

        nRet = ConvertFormat(inPic, videoFrame.width(), videoFrame.height(),
                             QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                             outPic, img.width(), img.height(),
                             AV_PIX_FMT_RGB24);
    }while (0);
    videoFrame.unmap();
    return img;
}

#elif HAVE_LIBYUV
QImage CImageTool::LibyuvConverFormatToRGB888(const QVideoFrame &frame)
{
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;
    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
        switch(videoFrame.pixelFormat())
        {
        case QVideoFrame::Format_YUV420P:
        {
            libyuv::I420ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width() / 2,
                                videoFrame.bits() +  videoFrame.width() * videoFrame.height() * 5 / 4,
                                videoFrame.width() / 2,
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
        }
            img = img.rgbSwapped();
            break;
        case QVideoFrame::Format_NV21:
        {
            libyuv::NV21ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width(),
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
        }
            break;
        case QVideoFrame::Format_NV12:
        {
            libyuv::NV12ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width(),
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
        }
            break;
        case QVideoFrame::Format_YV12:
        default:
            LOG_MODEL_WARNING("CImageTool",  "LibyuvConverFormatToRGB888 Don't implement conver format: %d",
                            videoFrame.pixelFormat());
        }
        
    }while(0);
    videoFrame.unmap();
    return img;
}
#endif

//参见： https://blog.csdn.net/junzia/article/details/76315120
void CImageTool::YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height)
{
	//找到Y、U、V在内存中的首地址  
	unsigned char* pY = pYUV;
	unsigned char* pU = pYUV + height * width;
	unsigned char* pV = pU + (height * width / 4);
 
 
	unsigned char* pBGR = nullptr;
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char Y = 0;
	unsigned char U = 0;
	unsigned char V = 0;
	double temp = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//找到相应的RGB首地址  
			pBGR = pRGB + i * width * 3 + j * 3;
 
			//取Y、U、V的数据值  
			Y = *(pY + i*width + j);
			U = *pU;
			V = *pV;
 
			//yuv转rgb公式  
			//yuv转rgb公式  
			temp = Y + ((1.773) * (U - 128));
			B = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			temp = (Y - (0.344) * (U - 128) - (0.714) * (V - 128));
			G = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			temp = (Y + (1.403)*(V - 128));
			R = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			//将转化后的rgb保存在rgb内存中，注意放入的顺序b是最低位  
			*pBGR = B;
			*(pBGR + 1) = G;
			*(pBGR + 2) = R;
 
 
			if (j % 2 != 0)
			{
				*pU++;
				*pV++;
			}
 
		}
		if (i % 2 == 0)
		{
			pU = pU - width / 2;
			pV = pV - width / 2;
		}
	}
}
