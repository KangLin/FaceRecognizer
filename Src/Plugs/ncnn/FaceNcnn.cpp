#include "FaceNcnn.h"
#include "DetectorNcnnRetina.h"

CFaceNcnn::CFaceNcnn(QObject *parent) : CFace(parent),
    m_DectorAlgorithm(NOT)
{}

CFaceNcnn::~CFaceNcnn()
{}

int CFaceNcnn::Initialize()
{
    setDectorAlgorithm();
    return 0;
}

QString CFaceNcnn::GetName()
{
    return "ncnn";
}

CFaceNcnn::DectorAlgorithm CFaceNcnn::getDectorAlgorithm()
{
    return m_DectorAlgorithm;
}

int CFaceNcnn::setDectorAlgorithm(DectorAlgorithm algorithm)
{
    if(m_DectorAlgorithm == algorithm)
        return 0;
    m_DectorAlgorithm = algorithm;
    if(m_pDetector) delete m_pDetector;
    
    switch (m_DectorAlgorithm) {
    case RETINA:
        m_pDetector = new CDetectorNcnnRetina(this);
        break;
    case NOT:
        Clean();
        break;
    }
    
    return 0;
}
