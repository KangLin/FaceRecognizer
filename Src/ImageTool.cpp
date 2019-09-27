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
#include <QtDebug>

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
    if(QVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QVideoFrame::Format_BGR24 == format)
        return AV_PIX_FMT_BGR24;
    else if(QVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QVideoFrame::Format_UYVY == format)
        return AV_PIX_FMT_UYVY422;
    else if(QVideoFrame::Format_NV21 == format)
        return AV_PIX_FMT_NV21;
    else if(QVideoFrame::Format_NV12 == format)
        return AV_PIX_FMT_NV12;
    else if(QVideoFrame::Format_YUV444 == format)
        return AV_PIX_FMT_YUYV422;

    return AV_PIX_FMT_NONE;
}

AVPixelFormat CImageTool::QImageFormatToFFMpegPixFormat(const QImage::Format format)
{
    if(QImage::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    return AV_PIX_FMT_NONE;
}

#ifdef RABBITIM_USE_QXMPP
AVPixelFormat CTool::QXmppVideoFrameFormatToFFMpegPixFormat(
        const QXmppVideoFrame::PixelFormat format)
{
    if(QXmppVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QXmppVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QXmppVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QXmppVideoFrame::Format_UYVY == format)
        return AV_PIX_FMT_UYVY422;
    else if(QXmppVideoFrame::Format_YUV420P == format)
        return AV_PIX_FMT_YUV420P;
    else
        return AV_PIX_FMT_NONE;
}

QXmppVideoFrame::PixelFormat CTool::QVideoFrameFormatToQXmppVideoFrameFormat(
        const QVideoFrame::PixelFormat format)
{
    if(QXmppVideoFrame::Format_RGB32 == format)
        return QXmppVideoFrame::Format_RGB32;
    else if(QXmppVideoFrame::Format_RGB24 == format)
        return QXmppVideoFrame::Format_RGB24;
    else if(QXmppVideoFrame::Format_YUYV == format)
        return QXmppVideoFrame::Format_YUYV;
    else if(QXmppVideoFrame::Format_UYVY == format)
        return QXmppVideoFrame::Format_UYVY;
    else if(QXmppVideoFrame::Format_YUV420P == format)
        return QXmppVideoFrame::Format_YUV420P;
    else
        return QXmppVideoFrame::Format_Invalid;
}

int CTool::ConvertFormat(const QXmppVideoFrame &inFrame,
                         QVideoFrame &outFrame,
                         int nOutWidth,
                         int nOutHeight,
                         QVideoFrame::PixelFormat outPixelFormat)
{
    int nRet = 0;
    AVPicture inPic, outPic;
    nRet = avpicture_fill(&inPic, (uint8_t*)inFrame.bits(),
                          QXmppVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                          inFrame.width(),
                          inFrame.height());
    if(nRet < 0)
    {
        LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
        return nRet;
    }

    int nByte = avpicture_fill(&outPic, NULL,
                          QVideoFrameFormatToFFMpegPixFormat(outPixelFormat),
                          nOutWidth,
                          nOutHeight);
    if(nByte < 0)
    {
        LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
        return -2;
    }
    QVideoFrame out(nByte, QSize(nOutWidth, nOutHeight), outPic.linesize[0], outPixelFormat);
    if(!out.map(QAbstractVideoBuffer::WriteOnly))
    {
        LOG_MODEL_ERROR("CTool", "outFrame map is fail");
        return -3;
    }
    do{
        nRet = avpicture_fill(&outPic, out.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(out.pixelFormat()),
                              out.width(),
                              out.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_file is fail:%d", nRet);
            nRet = -4;
            break;
        }
        nRet = ConvertFormat(inPic, inFrame.width(), inFrame.height(),
                             QXmppVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                             outPic, out.width(), out.height(),
                             QVideoFrameFormatToFFMpegPixFormat(outPixelFormat));
        if(!nRet)
            outFrame = out;
    }while(0);
    out.unmap();
    return nRet;
}

#endif

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

uchar RGBtoGRAY(uchar r, uchar g, uchar b)  
{  
    return (uchar)((((qint32)((r << 5) + (r << 2) + (r << 1)))
                    + (qint32)((g << 6) + (g << 3) + (g << 1) + g) 
                    + (qint32)((b << 4) - b)) >> 7);  
}  

QImage CImageTool::ConverFormatToRGB888(const QVideoFrame &frame)
{
    switch(frame.pixelFormat())
    {
    case QVideoFrame::Format_YUV420P:
    case QVideoFrame::Format_NV21:
    case QVideoFrame::Format_NV12:
    case QVideoFrame::Format_YV12:
        
#ifdef HAVE_OPENCV
        return OpenCVConverFormatToRGB888(frame);
#elif HAVE_LIBYUV
        return  LibyuvConverFormatToRGB888(frame);
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
            default:
                qDebug() << "Don't conver format:" << videoFrame.pixelFormat();
            }
        }
    }while(0);
    videoFrame.unmap();
    return img;
}

#ifdef HAVE_OPENCV
QImage CImageTool::OpenCVConverFormatToRGB888(const QVideoFrame &frame)
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
        cv::Mat out(videoFrame.height(), videoFrame.width(), CV_8UC3, img.bits());
        switch(videoFrame.pixelFormat())
        {
        case QVideoFrame::Format_YUV420P:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR == 3
            cv::cvtColor(in, out, CV_YUV420p2RGB);
#else
            cv::cvtColor(in, out, cv::COLOR_YUV420p2RGB);
#endif
        }
            break;
        case QVideoFrame::Format_NV21:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR == 3 
            cv::cvtColor(in, out, CV_YUV420sp2RGB);
#else
            cv::cvtColor(in, out, cv::COLOR_YUV420sp2RGB);
#endif
        }
            break;
        case QVideoFrame::Format_NV12:
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits());
#if OpenCV_VERSION_MAJOR == 3 
            cv::cvtColor(in, out, CV_YUV2RGB_NV12);
#else
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_NV12);
#endif
        }
            break;
        case QVideoFrame::Format_YV12:
        
        default:
            qDebug() << "Don't conver format:" << videoFrame.pixelFormat();
        }
        
    }while(0);
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
            qDebug() << "Don't conver format:" << videoFrame.pixelFormat();
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
