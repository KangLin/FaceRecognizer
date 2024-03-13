#ifndef CCONVERFORMAT_H
#define CCONVERFORMAT_H

#include <QObject>
#include <QVideoFrame>
#include "facerecognizer_export.h"

/*!
 * \~english
 * \defgroup API_CONVER_FORMAT Format conversion interface
 *
 * \~chinese
 * \defgroup API_CONVER_FORMAT 格式转换接口
 * \~
 * \ingroup API_INTERNAL
 */

/**
 * \~english Format conversion plug-in interface.
 *    It converts the format of the image, which is implemented by the plugin.
 *
 * \~chinese
 * \brief 格式转换插件接口。它转换图像的格式，由插件实现。
 * \~
 * \ingroup API_INTERNAL
 */
class FACERECOGNIZER_EXPORT CConverFormat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    
public:
    explicit CConverFormat(QObject *parent = nullptr);

    virtual QImage onConverFormatToRGB888(const QVideoFrame &frame) = 0;

    virtual int Initialize();
    virtual int Clean();

    virtual QString getName();
    virtual QString Detail();
};

#define CONVER_FORMAT_IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.ImageTool.ConverFormat"
Q_DECLARE_INTERFACE(CConverFormat, CONVER_FORMAT_IID)
#endif // CCONVERFORMAT_H
