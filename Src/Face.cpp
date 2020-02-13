#include "Face.h"
#include "Log.h"

CFace::CFace(QObject *parent) : QObject(parent)
{
    m_pDatabase = new CDatabase();
}

CFace::~CFace()
{
    if(m_pDatabase)
        delete m_pDatabase;
}

bool CFace::IsValid()
{
    if(!GetDector())
    {
        LOG_MODEL_ERROR("CFace", "CFace::GetDector is null");
        return false;
    }
    if(!GetTracker())
    {
        LOG_MODEL_ERROR("CFace", "CFace::GetTracker is null");
        return false;
    }
    if(!GetLandmarker())
    {
        LOG_MODEL_ERROR("CFace", "CFace::GetLandmarker is null");
        return false;
    }
    if(!GetRecognizer())
    {
        LOG_MODEL_ERROR("CFace", "CFace::GetRecognizer is null");
        return false;
    }
    if(!GetFaceTools())
    {
        LOG_MODEL_ERROR("CFace", "CFace::GetFaceTools is null");
        return false;
    }
    if(!GetDatabase())
    {
        LOG_MODEL_ERROR("CFace", "CFace::GetDatabase is null");
        return false;
    }
    return true;
}

CDatabase* CFace::GetDatabase()
{
    return m_pDatabase;
}
