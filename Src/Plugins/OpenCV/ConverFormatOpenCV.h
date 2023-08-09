#ifndef CIMAGETOOLOPENCV_H
#define CIMAGETOOLOPENCV_H

#include "ConverFormat.h"

class CConverFormatOpenCV final: public CConverFormat
{
    Q_OBJECT
public:
    explicit CConverFormatOpenCV(QObject *parent = nullptr);

    Q_INTERFACES(CConverFormat)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID CONVER_FORMAT_IID)
#endif

    virtual QImage onConverFormatToRGB888(const QVideoFrame &frame) override;
};

#endif // CIMAGETOOLOPENCV_H
