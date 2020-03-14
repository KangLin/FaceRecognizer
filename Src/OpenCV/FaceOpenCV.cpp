#include "FaceOpenCV.h"

#include <opencv2/opencv.hpp>

#ifdef HAVE_OPENCV_DNN
    #include "DetectorOpenCVDNN.h"
#endif
#ifdef HAVE_OPENCV_FACE
    #include "DetectorOpenCV.h"
    #include "LandmarkerOpenCV.h"
#endif

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent)
{
#ifdef HAVE_OPENCV_DNN
    m_pDetector = new CDetectorOpenCVDNN(this);
#endif

#ifdef HAVE_OPENCV_FACE
    if(!m_pDetector)
    {
        m_pDetector = new CDetectorOpenCV(this);
    }
    m_pLandmarker = new CLandmarkerOpenCV(this);
#endif
}
