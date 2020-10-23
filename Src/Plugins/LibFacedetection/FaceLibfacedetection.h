#ifndef CFACELIBFACEDETECTION_H
#define CFACELIBFACEDETECTION_H

#pragma once

#include "Face.h"

class CFaceLibFacedetection : public CFace
{
    Q_OBJECT
public:
    CFaceLibFacedetection(QObject *parent = nullptr);
    
    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID FACE_IID)
#endif

    virtual int Initialize() override; 
    QString GetName() override;
    int GetLevel() override; 
};

#endif // CFACELIBFACEDETECTION_H
