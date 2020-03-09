#ifndef CFACELIBFACEDETECTION_H
#define CFACELIBFACEDETECTION_H

#pragma once

#include "Face.h"

class CFaceLibfacedetection : public CFace
{
    Q_OBJECT
public:
    CFaceLibfacedetection(QObject *parent = nullptr);
};

#endif // CFACELIBFACEDETECTION_H
