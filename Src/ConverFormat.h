#ifndef CCONVERFORMAT_H
#define CCONVERFORMAT_H

#include <QObject>
#include <QVideoFrame>
#include "facerecognizer_export.h"

/**
 * @brief The CConverFormat class. It is convert format of image,
 *        implemented by plugin
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
};

#define CONVER_FORMAT_IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.ImageTool.ConverFormat"
Q_DECLARE_INTERFACE(CConverFormat, CONVER_FORMAT_IID)
#endif // CCONVERFORMAT_H
