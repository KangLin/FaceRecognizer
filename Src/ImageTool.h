#ifndef TOOL_H
#define TOOL_H

//#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//#pragma execution_character_set("utf-8")
//#endif

#ifdef HAVE_FFMPEG
extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#undef PixelFormat
}
#endif

#include "facerecognizer_export.h"

#include <QObject>
#include <memory>
#include <string>
#include <QWidget>
#include <QVideoFrame>
#include <QFileDialog>

/**
 * @brief 工具类。提供一此工具函数  
 * @ingroup RABBITIM_TOOL
 */
class FACERECOGNIZER_EXPORT CImageTool : public QObject
{
    Q_OBJECT
public:
    explicit CImageTool(QObject *parent = nullptr);

public:
    static void YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height);
    static QImage ConverFormatToRGB888(const QVideoFrame &frame);

#if HAVE_LIBYUV
    static QImage LibyuvConverFormatToRGB888(const QVideoFrame &frame);
#endif
#if HAVE_OPENCV
    static QImage OpenCVConverFormatToRGB888(const QVideoFrame &frame);
#endif
#ifdef HAVE_FFMPEG
    //设置ffmpeg日志输出  
    static int SetFFmpegLog();

    /**
     * @brief 格式映射  
     * @param format 
     * @return 
     */
    static AVPixelFormat QVideoFrameFormatToFFMpegPixFormat(
            const QVideoFrame::PixelFormat format);
    static AVPixelFormat QImageFormatToFFMpegPixFormat(
            const QImage::Format format);
    static QImage FFMpegConverFormatToRGB888(const QVideoFrame &frame);
    
private:
    /**
     * @brief 格式转换，这个函数只内部调用    
     *        
     * @param inFrame：要转换的帧（注意：需要调用者用 avpicture_fill 初始化。）  
     * @param nInWidth：转换帧的宽度  
     * @param nInHeight：转换帧的高度  
     * @param inPixelFormat：转换帧的格式  
     * @param outFrame：转换后的帧（注意：需要调用者用 avpicture_alloc 或 avpicture_fill 初始化。）    
     * @param nOutWidth：转换后帧的宽度  
     * @param nOutHeight：转换后帧的高度  
     * @param outPixelFormat：转换后帧的格式  
     * @return 
     */
    static int ConvertFormat(/*[in]*/  const AVPicture &inFrame,           /** 要转换的帧 */
                             /*[in]*/  int nInWidth,                 /** 要转换的帧的宽度 */
                             /*[in]*/  int nInHeight,                /** 要转换的帧的高度 */
                             /*[in]*/  AVPixelFormat inPixelFormat,  /** 要转换的帧的格式 */
                             /*[out]*/ AVPicture &outFrame,                /** 转换后的帧 */
                             /*[in]*/  int nOutWidth,                /** 转换后的帧的宽度 */
                             /*[in]*/  int nOutHeight,               /** 转换后的帧的高度 */  
                             /*[in]*/  AVPixelFormat outPixelFormat);/** 转换后的帧的格式 */
#endif

signals:

public slots:

};

#endif // TOOL_H
