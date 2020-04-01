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

int CFactoryFace::RegisterFace(CFace* pFace)
{
    if(!pFace) return -1;
    int nIndex = -1;
    
    foreach(auto d, m_Face)
    {
        nIndex++;
        if(d->GetLevel() > pFace->GetLevel())
            continue;
        if(d->GetLevel() == pFace->GetLevel())
        {
            if(d->GetName() == pFace->GetName())
            {
                d->Clean();
                m_Face[nIndex] = pFace;
                return 0;
            }
        }
        if(d->GetLevel() < pFace->GetLevel())
        {
            m_Face.insert(nIndex, pFace);
            return 0;
        }
    }

    if(nIndex >= m_Face.size() - 1)
        m_Face.push_back(pFace);
    
    return 0;
}

int CFactoryFace::RemoveFace(const QString &szName, CFace *pFace)
{
    int nIndex = 0;
    foreach(auto d, m_Face)
    {
        if(d->GetName() == pFace->GetName())
        {
            d->Clean();
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
            if(d->GetName() == szName)
            {
                m_CurrentLib = nIndex;
                break;
            }
        }

    return 0;
}

QVector<CFace*> CFactoryFace::GetLibType()
{
    return m_Face;
}

CFace* CFactoryFace::GetFace(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(m_CurrentLib >= 0 && m_CurrentLib < m_Face.size())
            return m_Face[m_CurrentLib];
        if(m_bOnlyUserCurrent)
            return nullptr;

        foreach(auto d, m_Face)
            if(d) return d;
        return nullptr;
    }

    foreach(auto d, m_Face)
        if(d->GetName() == szName) return d;
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
            if(d && d->GetDector())
                return d->GetDector();
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
            if(d && d->GetTracker())
                return d->GetTracker();
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
            if(d && d->GetLandmarker())
                return d->GetLandmarker();
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
            if(d && d->GetRecognizer())
                return d->GetRecognizer();
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
            if(d && d->GetFaceTools())
                return d->GetFaceTools();
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
            if(d && d->GetDatabase())
                return d->GetDatabase();
        }
    }
    return GetFace(szName) ? GetFace(szName)->GetDatabase() : nullptr;
}

int CFactoryFace::setModelPath(const QString &szPath)
{
    foreach(auto d, m_Face)
    {
        CFace* pFace = d;
        if(!pFace) continue;

        if(pFace->GetDector())
            pFace->GetDector()->setModelPath(szPath);
        if(pFace->GetLandmarker())
            pFace->GetLandmarker()->setModelPath(szPath);
        if(pFace->GetTracker())
            pFace->GetTracker()->setModelPath(szPath);
        if(pFace->GetRecognizer())
            pFace->GetRecognizer()->setModelPath(szPath);
        if(pFace->GetFaceTools())
            pFace->GetFaceTools()->setModelPath(szPath);
    }
    return 0;
}
