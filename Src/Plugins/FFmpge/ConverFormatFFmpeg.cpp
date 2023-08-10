#include "ConverFormatFFmpeg.h"
#include "Performance.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(logFFMPGE, "FFMPGE")

//设置日志的回调函数
void Log(void*, int, const char* fmt, va_list vl)
{
    qDebug(logFFMPGE, fmt, vl);
}

CConverFormatFFmpeg::CConverFormatFFmpeg(QObject *parent) : CConverFormat(parent)
{
    qInfo(logFFMPGE) << "FFmpeg version:" << av_version_info()
                      << "libavutil version:"
                      << av_version_info()
                      << "libswscale Version:"
                      << swscale_version();
                     // << swscale_configuration();
}

int CConverFormatFFmpeg::SetFFmpegLog()
{
    //在程序初始化时设置ffmpeg日志的回调函数
    av_log_set_callback(Log);
    return 0;
}

AVPixelFormat CConverFormatFFmpeg::QVideoFrameFormatToFFMpegPixFormat(
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
        qCritical(logFFMPGE, "Don't convert format: %d", format);
        return AV_PIX_FMT_NONE;
    }
}

AVPixelFormat CConverFormatFFmpeg::QImageFormatToFFMpegPixFormat(const QImage::Format format)
{
    if(QImage::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    return AV_PIX_FMT_NONE;
}

QImage CConverFormatFFmpeg::onConverFormatToRGB888(const QVideoFrame &frame)
{
    int nRet = 0;
    AVFrame* inPic = av_frame_alloc();
    AVFrame* outPic = av_frame_alloc();
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;
    PERFORMANCE(FFMpegConverFormatToRGB888)
    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
#if LIBAVUTIL_VERSION_MAJOR >= 55
        nRet = av_image_fill_arrays(outPic->data, outPic->linesize,
                                    img.bits(), AV_PIX_FMT_RGB24, img.width(), img.height(), 1);
#else
        nRet = avpicture_fill((AVPicture *)outPic, videoFrame.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                              videoFrame.width(),
                              videoFrame.height());
#endif
        if(nRet < 0)
        {
            qCritical(logFFMPGE) << "av_image_fill_arrays fail:" << nRet;
            break;
        }
#if LIBAVUTIL_VERSION_MAJOR >= 55
        nRet = av_image_fill_arrays(inPic->data, inPic->linesize,
                                    videoFrame.bits(),
                                    QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                                    videoFrame.width(),
                                    videoFrame.height(),
                                    1);
#else
        nRet = avpicture_fill((AVPicture *)inPic, videoFrame.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                              videoFrame.width(),
                              videoFrame.height());
#endif
        if(nRet < 0)
        {
            qCritical(logFFMPGE) << "avpicture_fill is fail";
            break;
        }
        
        ConvertFormat(inPic, videoFrame.width(), videoFrame.height(),
                      QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                      outPic, img.width(), img.height(),
                      AV_PIX_FMT_RGB24);
    }while (0);
    videoFrame.unmap();
    PERFORMANCE_ADD_TIME(FFMpegConverFormatToRGB888, "FFMpegConverFormatToRGB888")
    av_frame_free(&inPic);
    av_frame_free(&outPic);
    return img;
}

//如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存
//成功返回0，不成功返回非0
int CConverFormatFFmpeg::ConvertFormat(
#if LIBAVUTIL_VERSION_MAJOR >= 55
        /*[in]*/  AVFrame* inFrame,             /** 要转换的帧 */
#else
        /*[in]*/  const AVPicture* inFrame,          /** 要转换的帧 */
#endif
        /*[in]*/  int nInWidth,                 /** 要转换的帧的宽度 */
        /*[in]*/  int nInHeight,                /** 要转换的帧的高度 */
        /*[in]*/  AVPixelFormat inPixelFormat,  /** 要转换的帧的格式 */
#if LIBAVUTIL_VERSION_MAJOR >= 55
        /*[out]*/ AVFrame* outFrame,               /** 转换后的帧 */
#else
        /*[out]*/ AVPicture* outFrame,               /** 转换后的帧 */
#endif
        /*[in]*/  int nOutWidth,                /** 转换后的帧的宽度 */
        /*[in]*/  int nOutHeight,               /** 转换后的帧的高度 */
        /*[in]*/  AVPixelFormat outPixelFormat)/** 转换后的帧的格式 */
{
    int nRet = 0;
    struct SwsContext* pSwsCtx = NULL;

    if(inPixelFormat == outPixelFormat
        && nInWidth == nOutWidth
        && nInHeight == nOutHeight)
    {
#if LIBAVUTIL_VERSION_MAJOR >= 55
        av_frame_copy(outFrame, inFrame);
#else
        av_picture_copy(outFrame, inFrame, inPixelFormat, nInWidth, nInHeight);
#endif
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
        qCritical(logFFMPGE) << "sws_getContext false";
        return -3;
    }
    
    //进行图片转换
    nRet = sws_scale(pSwsCtx,
                     inFrame->data, inFrame->linesize,
                     0, nInHeight,
                     outFrame->data, outFrame->linesize);
    if(nRet < 0)
    {
        qCritical(logFFMPGE) << "sws_scale fail:" << nRet;
    }
    else
    {
        nRet = 0;
    }

    sws_freeContext(pSwsCtx);
    return nRet;
}

// FFMPEG API 变更： https://www.cnblogs.com/schips/p/12197116.html
