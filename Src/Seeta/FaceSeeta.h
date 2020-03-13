#ifndef CFACESEETA_H_KL_2019_10_21
#define CFACESEETA_H_KL_2019_10_21

#pragma once

#include "Face.h"

class CFaceSeeta : public CFace
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    CFaceSeeta(QObject* parent = nullptr);
    virtual ~CFaceSeeta();
};

#endif // CFACESEETA_H_KL_2019_10_21
