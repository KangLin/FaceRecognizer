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
    m_CurrentLib(AUTO),
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
    m_CurrentLib = type;
    m_bOnlyUserCurrent = bOnly;
    return 0;
}

CFace* CFactoryFace::GetFace(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(m_bOnlyUserCurrent)
            return m_Face[m_CurrentLib];
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]) return m_Face[i];
        }
        return nullptr;
    }
    return m_Face[type];
}

CDetector* CFactoryFace::GetDector(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(GetFace(m_CurrentLib))
        {
            CDetector* pDetect = GetFace(m_CurrentLib)->GetDector();
            if(pDetect || m_bOnlyUserCurrent)
                return pDetect;
        }
        //TODO: 优化：使用性能高的库
        
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i] && m_Face[i]->GetDector())
                return m_Face[i]->GetDector();
        }
    }
    
    return GetFace(type) ? GetFace(type)->GetDector() : nullptr;
}

CTracker* CFactoryFace::GetTracker(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(GetFace(m_CurrentLib))
        {
            CTracker* pTracker = GetFace(m_CurrentLib)->GetTracker();
            if(pTracker || m_bOnlyUserCurrent)
                return pTracker;
        }
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i] && m_Face[i]->GetTracker())
                return m_Face[i]->GetTracker();
        }
    }
    return GetFace(type) ? GetFace(type)->GetTracker() : nullptr;
}

CLandmarker* CFactoryFace::GetLandmarker(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(GetFace(m_CurrentLib))
        {
            CLandmarker* pLandmarker = GetFace(m_CurrentLib)->GetLandmarker();
            if(pLandmarker || m_bOnlyUserCurrent)
                return pLandmarker;
        }
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i] && m_Face[i]->GetLandmarker())
                return m_Face[i]->GetLandmarker();
        }
    }
    return GetFace(type) ? GetFace(type)->GetLandmarker() : nullptr;
}

CRecognizer* CFactoryFace::GetRecognizer(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(GetFace(m_CurrentLib))
        {
            CRecognizer* pRecognizer = GetFace(m_CurrentLib)->GetRecognizer();
            if(pRecognizer || m_bOnlyUserCurrent)
                return pRecognizer;
        }
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i] && m_Face[i]->GetRecognizer())
                return m_Face[i]->GetRecognizer();
        }
    }

    return GetFace(type) ? GetFace(type)->GetRecognizer() : nullptr;
}

CFaceTools* CFactoryFace::GetFaceTools(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(GetFace(m_CurrentLib))
        {
            CFaceTools* pTools = GetFace(m_CurrentLib)->GetFaceTools();
            if(pTools || m_bOnlyUserCurrent)
                return pTools;
        }
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i] && m_Face[i]->GetFaceTools())
                return m_Face[i]->GetFaceTools();
        }
    }
    return GetFace(type) ? GetFace(type)->GetFaceTools() : nullptr;
}

CDatabase* CFactoryFace::GetDatabase(LIB_TYPE type)
{
    if(AUTO == type)
    {
        if(GetFace(m_CurrentLib))
        {
            CDatabase* pDb = GetFace(m_CurrentLib)->GetDatabase();
            if(pDb || m_bOnlyUserCurrent)
                return pDb;
        }
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i] && m_Face[i]->GetDatabase())
                return m_Face[i]->GetDatabase();
        }
    }
    return GetFace(type) ? GetFace(type)->GetDatabase() : nullptr;
}
