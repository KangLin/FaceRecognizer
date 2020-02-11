#ifndef DETECTOROPENCV_H
#define DETECTOROPENCV_H

#include "Detector.h"

class CDetectorOpenCV : public CDetector
{
    Q_OBJECT
public:
    explicit CDetectorOpenCV(QObject *parent = nullptr);
    virtual ~CDetectorOpenCV();

    virtual int Detect(const QImage &image,  QVector<QRect> &faces);
    
private:
    virtual int UpdateParameter(QString &szErr);    

};

#endif // DETECTOROPENCV_H
