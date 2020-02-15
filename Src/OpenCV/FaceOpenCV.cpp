#include "FaceOpenCV.h"
#include "ParameterFactory.h"

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
    CParameterFactory* pFactory = CParameterFactory::Instance();

#ifdef HAVE_OPENCV_DNN
    m_pDetector = new CDetectorOpenCVDNN(this);
    if(m_pDetector) m_pDetector->SetParameter(pFactory->GetParameterDetector());
#endif

#ifdef HAVE_OPENCV_FACE
    if(!m_pDetector)
    {
        m_pDetector = new CDetectorOpenCV(this);
        if(m_pDetector) m_pDetector->SetParameter(pFactory->GetParameterDetector());
    }
    m_pLandmarker = new CLandmarkerOpenCV(this);
    if(m_pLandmarker)
        m_pLandmarker->SetParameter(pFactory->GetParameterLandmark());
#endif
}
