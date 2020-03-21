#ifndef CFACELIBFACEDETECTION_H
#define CFACELIBFACEDETECTION_H

#pragma once

#include "Face.h"

class CFaceLibfacedetection : public CFace
{
    Q_OBJECT
public:
    CFaceLibfacedetection(QObject *parent = nullptr);
    
    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.Libfacedetection.CFaceLibfacedetection")
#endif

    int Initialize(CFactoryFace* pFactoryFace = nullptr) override;
    int Clean(CFactoryFace* pFactoryFace = nullptr) override;
};

#endif // CFACELIBFACEDETECTION_H
