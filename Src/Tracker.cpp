#include "Tracker.h"

static int gTrackStrFaceId = qRegisterMetaType<CTracker::strFace>();
static int gVectorTrackStrFaceId = qRegisterMetaType<QVector<CTracker::strFace> >();
CTracker::CTracker(CFace *pFace, QObject *parent) : CFaceBase(parent),
    m_pFace(pFace)
{}
