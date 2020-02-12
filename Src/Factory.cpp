#include "Factory.h"
#include "Log.h"
#ifdef HAVE_SEETA_FACE
    #include "Seeta/FaceSeeta.h"
#endif
#ifdef HAVE_OPENCV
    #include "OpenCV/FaceOpenCV.h"
#endif
#include "ParameterFactory.h"

CFactory::CFactory(QObject *parent)
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
}

CFactory::~CFactory()
{
}

CFactory* CFactory::Instance()
{
    static CFactory* p = nullptr;
    if(!p)
        p = new CFactory();
    return p;
}

bool CFactory::bIsValid(LIB_TYPE type)
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

CFace* CFactory::GetFace(LIB_TYPE type)
{
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]) return m_Face[i];
        }
    
    return m_Face[type];
}

CDetector* CFactory::GetDector(LIB_TYPE type)
{
    CDetector* p = GetFace(type)->GetDector();
    if(p) return p;
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetDector()) return m_Face[i]->GetDector();
        }
    return nullptr;
}

CTracker* CFactory::GetTracker(LIB_TYPE type)
{
    CTracker* p = GetFace(type)->GetTracker();
    if(p) return p;
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetTracker()) return m_Face[i]->GetTracker();
        }
    return nullptr;
}

CLandmarker* CFactory::GetLandmarker(LIB_TYPE type)
{
    CLandmarker* p = GetFace(type)->GetLandmarker();
    if(p) return p;
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetLandmarker()) return m_Face[i]->GetLandmarker();
        }

    return nullptr;
}

CRecognizer* CFactory::GetRecognizer(LIB_TYPE type)
{
    CRecognizer* p = GetFace(type)->GetRecognizer();
    if(p) return p;
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetRecognizer()) return m_Face[i]->GetRecognizer();
        }

    return nullptr;
}

CFaceTools* CFactory::GetFaceTools(LIB_TYPE type)
{
    CFaceTools* p = GetFace(type)->GetFaceTools();
    if(p) return p;
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetFaceTools()) return m_Face[i]->GetFaceTools();
        }

    return nullptr;
}

CDatabase* CFactory::GetDatabase(LIB_TYPE type)
{
    CDatabase* p = GetFace(type)->GetDatabase();
    if(p) return p;
    if(AUTO == type)
        for (int i = 0; i < AUTO; i++) {
            if(m_Face[i]->GetDatabase()) return m_Face[i]->GetDatabase();
        }

    return nullptr;
}
