#include "FactoryFace.h"
#include "Log.h"
#ifdef HAVE_SEETA_FACE
    #include "Seeta/FaceSeeta.h"
#endif
#ifdef HAVE_OPENCV
    #include "OpenCV/FaceOpenCV.h"
#endif
#include "ParameterFactory.h"

CFactoryFace::CFactoryFace(QObject *parent): QObject(parent),
    m_CurrentLIb(AUTO),
    m_bOnlyUserCurrent(false)
{
    Q_UNUSED(parent)
    for (int i = 0; i < AUTO; i++) {
        m_Face[i] = nullptr;
    }

#ifdef HAVE_OPENCV
    m_Face[OPENCV] = new CFaceOpenCV();
#endif

#ifdef HAVE_SEETA_FACE
    m_Face[SEETA] = new CFaceSeeta();
#endif

    SetLibType(AUTO, false);
}

CFactoryFace::~CFactoryFace()
{
}

CFactoryFace* CFactoryFace::Instance()
{
    static CFactoryFace* p = nullptr;
    if(!p)
        p = new CFactoryFace();
    return p;
}

bool CFactoryFace::bIsValid(LIB_TYPE type)
{
    if(!GetFace(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetFace is null");
        return false;
    }
    if(!GetDector(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetDector is null");
        return false;
    }
    if(!GetTracker(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetTracker is null");
        return false;
    }
    if(!GetLandmarker(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetLandmarker is null");
        return false;
    }
    if(!GetRecognizer(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetRecognizer is null");
        return false;
    }
    if(!GetFaceTools(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetFaceTools is null");
        return false;
    }
    if(!GetDatabase(type))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetDatabase is null");
        return false;
    }
    return true;
}

int CFactoryFace::SetLibType(LIB_TYPE type, bool bOnly)
{
    m_CurrentLIb = type;
    m_bOnlyUserCurrent = bOnly;
    return 0;
}

CFace* CFactoryFace::GetFace(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return m_Face[m_CurrentLIb];
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]) return m_Face[i];
        }
    }
    return m_Face[type];
}

CDetector* CFactoryFace::GetDector(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return GetFace(m_CurrentLIb)->GetDector();
        
        //TODO: 优化：使用性能高
        
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetDector()) return m_Face[i]->GetDector();
        }
    }
    
    return GetFace(type)->GetDector();
}

CTracker* CFactoryFace::GetTracker(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return GetFace(m_CurrentLIb)->GetTracker();
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetTracker()) return m_Face[i]->GetTracker();
        }
    }
    return GetFace(type)->GetTracker();
}

CLandmarker* CFactoryFace::GetLandmarker(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return GetFace(m_CurrentLIb)->GetLandmarker();
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetLandmarker()) return m_Face[i]->GetLandmarker();
        }
    }
    return GetFace(type)->GetLandmarker();
}

CRecognizer* CFactoryFace::GetRecognizer(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return GetFace(m_CurrentLIb)->GetRecognizer();
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetRecognizer()) return m_Face[i]->GetRecognizer();
        }
    }

    return GetFace(type)->GetRecognizer();
}

CFaceTools* CFactoryFace::GetFaceTools(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return GetFace(m_CurrentLIb)->GetFaceTools();
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetFaceTools()) return m_Face[i]->GetFaceTools();
        }
    }
    return GetFace(type)->GetFaceTools();
}

CDatabase* CFactoryFace::GetDatabase(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return GetFace(m_CurrentLIb)->GetDatabase();
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetDatabase()) return m_Face[i]->GetDatabase();
        }
    }
    return GetFace(type)->GetDatabase();
}
