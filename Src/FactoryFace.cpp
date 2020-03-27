#include "FactoryFace.h"
#include "Log.h"

#ifdef RABBITCOMMON
    #include <RabbitCommonDir.h>
#endif

#include <QDebug>

CFactoryFace::CFactoryFace(QObject *parent): QObject(parent),
    m_CurrentLib(-1),
    m_bOnlyUserCurrent(false)
{
    Q_UNUSED(parent)
    
    SetLibType(QString(), false);
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

int CFactoryFace::RegisterFace(const QString &szName,
                               CFace* pFace,
                               const QString &szDescript)
{
    int nIndex = -1;
    FACE_DATA data{szName, szDescript, pFace};
    foreach(auto d, m_Face)
    {
        nIndex++;
        if(d.szName == szName)
        {
            d.pFace->Clean(this);
            break;
        }
    }

    if(nIndex >= m_Face.size() - 1)
        m_Face.push_back(data);
    else
        m_Face[nIndex] = data;

    return 0;
}

int CFactoryFace::RemoveFace(const QString &szName)
{
    int nIndex = 0;
    foreach(auto d, m_Face)
    {
        if(d.szName == szName)
        {
            d.pFace->Clean(this);
            m_Face.remove(nIndex);
            break;
        }
        nIndex++;
    }
    if(m_CurrentLib >= m_Face.size())
        m_CurrentLib = m_Face.size() - 1;
    return 0;
}

bool CFactoryFace::bIsValid(const QString &szName)
{
    if(!GetFace(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetFace is null");
        return false;
    }
    if(!GetDector(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetDector is null");
        return false;
    }
    if(!GetTracker(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetTracker is null");
        return false;
    }
    if(!GetLandmarker(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetLandmarker is null");
        return false;
    }
    if(!GetRecognizer(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetRecognizer is null");
        return false;
    }
    if(!GetFaceTools(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetFaceTools is null");
        return false;
    }
    if(!GetDatabase(szName))
    {
        LOG_MODEL_ERROR("CFactory", "CFactory::GetDatabase is null");
        return false;
    }
    return true;
}

int CFactoryFace::SetLibType(const QString &szName, bool bOnly)
{
    int nIndex = -1;
    if(0 < m_Face.size())
        m_CurrentLib = 0;
    else
        m_CurrentLib = -1;
    m_bOnlyUserCurrent = bOnly;
    
    if(!szName.isEmpty())
        foreach(auto d, m_Face)
        {
            nIndex++;
            if(d.szName == szName)
            {
                m_CurrentLib = nIndex;
                break;
            }
        }

    return 0;
}

int CFactoryFace::GetLibType(QVector<QString> &szLibs, QVector<QString> &szDescript)
{
    foreach(auto d, m_Face)
    {
        szDescript.push_back(d.szDescript);
        szLibs.push_back(d.szName);
    }
    return 0;
}

CFace* CFactoryFace::GetFace(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(m_CurrentLib >= 0 && m_CurrentLib < m_Face.size())
            return m_Face[m_CurrentLib].pFace;
        if(m_bOnlyUserCurrent)
            return nullptr;

        foreach(auto d, m_Face)
            if(d.pFace) return d.pFace;
        return nullptr;
    }

    foreach(auto d, m_Face)
        if(d.szName == szName) return d.pFace;
    return nullptr;
}

CDetector* CFactoryFace::GetDector(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(GetFace(szName))
        {
            CDetector* pDetect = GetFace(szName)->GetDector();
            if(pDetect)
                return pDetect;
        }
        
        if(m_bOnlyUserCurrent)
            return nullptr;
        
        //TODO: 优化：使用性能高的库
        
        foreach(auto d, m_Face) {
            if(d.pFace && d.pFace->GetDector())
                return d.pFace->GetDector();
        }
    }
    
    return GetFace(szName) ? GetFace(szName)->GetDector() : nullptr;
}

CTracker* CFactoryFace::GetTracker(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(GetFace(szName))
        {
            CTracker* pTracker = GetFace(szName)->GetTracker();
            if(pTracker)
                return pTracker;
        }
        if(m_bOnlyUserCurrent)
            return nullptr;
        
        foreach(auto d, m_Face) {
            if(d.pFace && d.pFace->GetTracker())
                return d.pFace->GetTracker();
        }
    }
    return GetFace(szName) ? GetFace(szName)->GetTracker() : nullptr;
}

CLandmarker* CFactoryFace::GetLandmarker(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(GetFace(szName))
        {
            CLandmarker* pLandmarker = GetFace(szName)->GetLandmarker();
            if(pLandmarker)
                return pLandmarker;
        }
        if(m_bOnlyUserCurrent)
            return nullptr;
        
        foreach(auto d, m_Face) {
            if(d.pFace && d.pFace->GetLandmarker())
                return d.pFace->GetLandmarker();
        }
    }
    return GetFace(szName) ? GetFace(szName)->GetLandmarker() : nullptr;
}

CRecognizer* CFactoryFace::GetRecognizer(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(GetFace(szName))
        {
            CRecognizer* pRecognizer = GetFace(szName)->GetRecognizer();
            if(pRecognizer)
                return pRecognizer;
        }
        if(m_bOnlyUserCurrent)
            return nullptr;
        
        foreach(auto d, m_Face) {
            if(d.pFace && d.pFace->GetRecognizer())
                return d.pFace->GetRecognizer();
        }
    }

    return GetFace(szName) ? GetFace(szName)->GetRecognizer() : nullptr;
}

CFaceTools* CFactoryFace::GetFaceTools(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(GetFace(szName))
        {
            CFaceTools* pTools = GetFace(szName)->GetFaceTools();
            if(pTools)
                return pTools;
        }
        if(m_bOnlyUserCurrent)
            return nullptr;
        
        foreach(auto d, m_Face) {
            if(d.pFace && d.pFace->GetFaceTools())
                return d.pFace->GetFaceTools();
        }
    }
    return GetFace(szName) ? GetFace(szName)->GetFaceTools() : nullptr;
}

CDatabase* CFactoryFace::GetDatabase(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(GetFace(szName))
        {
            CDatabase* pDb = GetFace(szName)->GetDatabase();
            if(pDb)
                return pDb;
        }
        if(m_bOnlyUserCurrent)
            return nullptr;
        
        foreach(auto d, m_Face) {
            if(d.pFace && d.pFace->GetDatabase())
                return d.pFace->GetDatabase();
        }
    }
    return GetFace(szName) ? GetFace(szName)->GetDatabase() : nullptr;
}
