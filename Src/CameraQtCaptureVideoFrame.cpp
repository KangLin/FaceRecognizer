#include "CameraQtCaptureVideoFrame.h"
#include "ImageTool.h"
#include "Performance.h"

#include <QThread>
#include <QTime>
#include <QVideoFrame>
#include <QImage>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "Camera")

CCameraQtCaptureVideoFrame::CCameraQtCaptureVideoFrame(QObject *parent) :
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    QVideoSink(parent)
#else
    QAbstractVideoSurface(parent)
#endif
{
    m_Angle = 0;
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    bool check = connect(this, SIGNAL(videoFrameChanged(const QVideoFrame&)),
                         this, SLOT(present(const QVideoFrame&)));
    Q_ASSERT(check);
#endif
}

CCameraQtCaptureVideoFrame::~CCameraQtCaptureVideoFrame()
{
    qDebug() << "CCameraQtCaptureVideoFrame::~CCameraQtCaptureVideoFrame()";
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//选择需要捕获视频帧的格式  
QList<QVideoFrame::PixelFormat> 
CCameraQtCaptureVideoFrame::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB24
                << QVideoFrame::Format_RGB32  //windows 平台、linux 平台默认都支持 RGB32 格式  
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555
                
                << QVideoFrame::Format_BGR32
                << QVideoFrame::Format_BGR24
                << QVideoFrame::Format_BGR565
                << QVideoFrame::Format_BGR555
                << QVideoFrame::Format_BGRA32

                //android支持的格式
                // YUV420SP
                << QVideoFrame::Format_NV12
                << QVideoFrame::Format_NV21
                   
                // YUV420P
                << QVideoFrame::Format_YUV420P
                << QVideoFrame::Format_YV12
                
                << QVideoFrame::Format_UYVY
                << QVideoFrame::Format_YUYV
                << QVideoFrame::Format_YUV444
                << QVideoFrame::Format_AYUV444
                //<< QVideoFrame::Format_Jpeg
                //<< QVideoFrame::Format_CameraRaw
                //<< QVideoFrame::Format_User
                ;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
#endif //#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
void CCameraQtCaptureVideoFrame::present(const QVideoFrame &frame)
#else
bool CCameraQtCaptureVideoFrame::present(const QVideoFrame &frame)
#endif
{
    //qDebug() << "Frame pixel format:" << frame.pixelFormat();
    emit sigCaptureFrame(frame);
    PERFORMANCE(present)
    QImage img = CImageTool::Instance()->ConverFormatToRGB888(frame);
    PERFORMANCE_ADD_TIME(present,
             "convert format to RBG888, rotation:" + QString::number(m_Angle))
    if(m_Angle)
        img = img.transformed(QTransform().rotate(-1 * m_Angle));
    emit sigCaptureFrame(img);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return true;
#endif
}

int CCameraQtCaptureVideoFrame::SetCameraAngle(int angle)
{
    m_Angle = angle;
    return 0;
}
