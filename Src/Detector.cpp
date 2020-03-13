#include "Detector.h"

CDetector::CDetector(CFace *pFace, QObject *parent) : CFaceBase(parent),
    m_pFace(pFace)
{}

CDetector::~CDetector()
{}
