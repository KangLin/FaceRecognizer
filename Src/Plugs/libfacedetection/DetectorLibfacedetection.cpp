#include "DetectorLibfacedetection.h"
#include "facedetection/facedetectcnn.h"
#include "Performance.h"

CDetectorLibfacedetection::CDetectorLibfacedetection(CFace *pFace, QObject *parent)
    : CDetector(pFace, parent)
{}

int CDetectorLibfacedetection::UpdateParameter()
{
    return 0;
}

int CDetectorLibfacedetection::Detect(const QImage &image, QVector<QRect> &faces)
{
    if(image.isNull()) return -1;
    
    PERFORMANCE(Libfacedetection)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(Libfacedetection)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(Libfacedetection,
                             "Libfacedetection format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    img = img.rgbSwapped();
    PERFORMANCE_ADD_TIME(Libfacedetection, "rgbSwapped")
       
    faces.clear();
    //define the buffer size. Do not change the size!
    #define DETECT_BUFFER_SIZE 0x20000
    unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
    PERFORMANCE_START(Libfacedetection)
    int *pResults  = facedetect_cnn(pBuffer,
                                    (unsigned char*)img.bits(), img.width(),
                                    img.height(),
                                    img.width() * 3);
    PERFORMANCE_ADD_TIME(Libfacedetection, "detect:image width:"
                         + QString::number(image.width())
                         + ";Height:"
                         + QString::number(image.height()))
            
    //print the detection results
    for(int i = 0; i < (pResults ? *pResults : 0); i++)
    {
        short * p = ((short*)(pResults+1))+142*i;
        int confidence = p[0];
        QRect r(p[1], p[2], p[3], p[4]);
        faces.push_back(r);        
    }
    
    PERFORMANCE_ADD_TIME(Libfacedetection, "copy reture value")
    if(pBuffer) free(pBuffer);
    return 0;
}
