#include "FaceOpenCV.h"
#include "DetectorOpenCV.h"
#include "LandmarkerOpenCV.h"
#include "DetectorOpenCVDNN.h"
#include <QDebug>

CFaceOpenCV::CFaceOpenCV(QObject *parent) : CFace(parent),
    m_Algorithm(NOT)
{}

int CFaceOpenCV::Initialize()
{
    setAlgorithm();
    return 0;    
}

QString CFaceOpenCV::GetName()
{
    return "OpencvFace";
}

CFaceOpenCV::Algorithm CFaceOpenCV::getAlgorithm()
{
    return m_Algorithm;
}

int CFaceOpenCV::setAlgorithm(Algorithm algorithm)
{
    qDebug() << "CFaceOpenCV::setAlgorithm" << algorithm;
    if(m_Algorithm == algorithm)
        return 0;
    m_Algorithm = algorithm;

    switch (algorithm) {
    case OPENCV:
        if(m_pDetector) delete m_pDetector;
        m_pDetector = new CDetectorOpenCV(this);
        if(m_pLandmarker) delete m_pLandmarker;
        m_pLandmarker = new CLandmarkerOpenCV(this);
        break;
    case DNN:
        if(m_pDetector) delete m_pDetector;
        m_pDetector = new CDetectorOpenCVDNN(this);
        break;
    case NOT:
        Clean();
        break;
    }
    
    return 0;
}
