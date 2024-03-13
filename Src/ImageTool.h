/*
  Author: Kang Lin <kl222@126.com>
*/

#ifndef TOOL_H
#define TOOL_H

//#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//#pragma execution_character_set("utf-8")
//#endif

#include "facerecognizer_export.h"

#include <QObject>
#include <memory>
#include <string>
#include <QWidget>
#include <QVideoFrame>
#include <QFileDialog>
#include <QMetaClassInfo>

#include "ConverFormat.h"

/**
 * \~english Manage the plugins of image tool
 *
 * \~chinese 管理图像转换插件工具
 *
 * \~
 * \ingroup API
 */
class FACERECOGNIZER_EXPORT CImageTool : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
public:
    explicit CImageTool(QObject *parent = nullptr);
    static CImageTool* Instance();

public:
    static void YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height);
    QImage ConverFormatToRGB888(const QVideoFrame &frame);
    
    QString Detail();

private:
    CConverFormat* m_pConverFormat;
    
    int FindPlugins(QDir dir, QStringList filters);
    
#if HAVE_LIBYUV
    static QImage LibyuvConverFormatToRGB888(const QVideoFrame &frame);
#endif

};

#endif // TOOL_H
