#include "FaceNcnn.h"
#include "DetectorNcnnRetina.h"

CFaceNcnn::CFaceNcnn(QObject *parent) : CFace(parent)
{}

CFaceNcnn::~CFaceNcnn()
{}

int CFaceNcnn::Initialize()
{
    m_pDetector = new CDetectorNcnnRetina(this);
    return 0;
}

QString CFaceNcnn::GetName()
{
    return "ncnn";
}
