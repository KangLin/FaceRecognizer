#ifndef CFACEOPENCVDNN_H
#define CFACEOPENCVDNN_H

#include "Face.h"
class CFaceOpenCVDNN : public CFace
{
    Q_OBJECT
public:
    explicit CFaceOpenCVDNN(QObject *parent = nullptr);
    
    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.OpenCV.CFaceOpenCVDNN")
#endif

    virtual int Initialize() override;
    QString GetName() override; 
};

#endif // CFACEOPENCVDNN_H
