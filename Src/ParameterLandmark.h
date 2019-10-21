#ifndef CPARAMTERLANDMARK_H
#define CPARAMTERLANDMARK_H

#include <QObject>
#include "Parameter.h"

class FACERECOGNIZER_EXPORT CParameterLandmark : public CParameter
{
    Q_OBJECT
public:
    explicit CParameterLandmark(QObject *parent = nullptr);
    int SetPoints(int points);
    int GetPoints();
    
private:
    int m_Points;
};

#endif // CPARAMTERLANDMARK_H
