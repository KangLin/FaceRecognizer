#ifndef CCONVERFORMATFFMPEG_H
#define CCONVERFORMATFFMPEG_H

#include "ConverFormat.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#undef PixelFormat
}

/*!
 * \brief The CConverFormatFFmpeg class
 * \~
 * \ingroup API_CONVER_FORMAT
 */
class CConverFormatFFmpeg final: public CConverFormat
{
    Q_OBJECT
public:
    explicit CConverFormatFFmpeg(QObject *parent = nullptr);
    virtual QImage onConverFormatToRGB888(const QVideoFrame &frame);

    Q_INTERFACES(CConverFormat)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID CONVER_FORMAT_IID)
#endif

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
    static int ConvertFormat(
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
        /*[in]*/  AVPixelFormat outPixelFormat);/** 转换后的帧的格式 */

};

#endif // CCONVERFORMATFFMPEG_H
