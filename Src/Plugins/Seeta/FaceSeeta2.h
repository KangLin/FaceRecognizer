#ifndef CFACESEETA_H_KL_2019_10_21
#define CFACESEETA_H_KL_2019_10_21

#pragma once

#include "Face.h"

class CFaceSeeta2 final: public CFace
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    CFaceSeeta2(QObject* parent = nullptr);
    virtual ~CFaceSeeta2() override;
    
    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID FACE_IID)
#endif

    virtual int Initialize() override;
    QString GetName() override;
    int GetLevel() override;
    
    const QString Detail() const override;
};

#endif // CFACESEETA_H_KL_2019_10_21
