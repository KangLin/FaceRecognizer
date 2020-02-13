#ifndef FACEOPENCV_H
#define FACEOPENCV_H

#include "Face.h"

class CFaceOpenCV : public CFace
{
    Q_OBJECT
public:
    explicit CFaceOpenCV(QObject *parent = nullptr);
};

#endif // FACEOPENCV_H
