#ifndef CCONVERFORMAT_H
#define CCONVERFORMAT_H

#include <QObject>
#include <QVideoFrame>

class CImageTool;
class CConverFormat : public QObject
{
    Q_OBJECT
public:
    explicit CConverFormat(QObject *parent = nullptr);

    virtual QImage onConverFormatToRGB888(const QVideoFrame &frame) = 0;

    virtual int Initialize(CImageTool* pTool);
    virtual int Clean(CImageTool* pTool = nullptr);

    virtual QString getName();
};

Q_DECLARE_INTERFACE(CConverFormat, "KangLinStudio.Rabbit.FaceRecognizer.Plugs.ImageTool.ConverFormat")
#endif // CCONVERFORMAT_H
