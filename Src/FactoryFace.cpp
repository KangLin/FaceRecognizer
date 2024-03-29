#include "FactoryFace.h"

#ifdef RABBITCOMMON
    #include <RabbitCommonDir.h>
#endif

#include <QDebug>
#include <QPluginLoader>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "Face.Factory")

CFactoryFace::CFactoryFace(QObject *parent): QObject(parent),
    m_CurrentLib(-1),
    m_bOnlyUserCurrent(true)
{    
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        CFace* pPlugFace = qobject_cast<CFace*>(plugin);
        if(pPlugFace)
        {
            RegisterFace(pPlugFace);
        }
    }

    QString szPath = RabbitCommon::CDir::Instance()->GetDirPlugins();
#if !defined (Q_OS_ANDROID)
    szPath = szPath + QDir::separator() + "Face";
#endif
    QStringList filters;
#if defined (Q_OS_WINDOWS) || defined(Q_OS_WIN)
        filters << "*PluginFace*.dll";
#else
        filters << "*PluginFace*.so";
#endif
    FindPlugins(szPath, filters);

    SetLibType(QString(), false);
}

CFactoryFace::~CFactoryFace()
{
}

int CFactoryFace::SetLibType(const QString &szName, bool bOnly)
{
    int nIndex = -1;
    int nOldCurr = m_CurrentLib;
    
    if(m_bOnlyUserCurrent != bOnly)
    {
        foreach(auto d, m_Faces)
            if(m_bOnlyUserCurrent)
            {
                if(d)
                {
                    d->Clean();
                    d->Initialize();
                }
            } else {
                if(d) d->Clean();
            }
    }

    m_bOnlyUserCurrent = bOnly;

    if(szName.isEmpty())
    {
        if(0 < m_Faces.size() && -1 == m_CurrentLib)
            m_CurrentLib = 0;
    }
    else
    {
        foreach(auto d, m_Faces)
        {
            nIndex++;
            if(d->GetName() == szName)
            {
                m_CurrentLib = nIndex;
                break;
            }
        }
    }

    if(nOldCurr != m_CurrentLib && m_bOnlyUserCurrent)
    {
        if(nOldCurr >= 0 && nOldCurr < m_Faces.size())
        {
            CFace *f = m_Faces.at(nOldCurr);
            if(f) f->Clean();    
        }

        CFace *f = GetFace();
        if(f) f->Initialize();
    }

    return 0;
}

QVector<CFace*> CFactoryFace::GetLibType()
{
    return m_Faces;
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
    
    foreach(auto d, m_Faces)
    {
        nIndex++;
        if(d->GetLevel() > pFace->GetLevel())
            continue;
        if(d->GetLevel() == pFace->GetLevel())
        {
            if(d->GetName() == pFace->GetName())
            {
                d->Clean();
                m_Faces[nIndex] = pFace;
                return 0;
            }
        }
        if(d->GetLevel() < pFace->GetLevel())
        {
            m_Faces.insert(nIndex, pFace);
            return 0;
        }
    }

    if(nIndex >= m_Faces.size() - 1)
        m_Faces.push_back(pFace);

    return 0;
}

int CFactoryFace::RemoveFace(const QString &szName, CFace *pFace)
{
    int nIndex = 0;
    foreach(auto d, m_Faces)
    {
        if(d->GetName() == pFace->GetName())
        {
            d->Clean();
            m_Faces.remove(nIndex);
            break;
        }
        nIndex++;
    }
    if(m_CurrentLib >= m_Faces.size())
        m_CurrentLib = m_Faces.size() - 1;
    return 0;
}

bool CFactoryFace::bIsValid(const QString &szName)
{
    if(!GetFace(szName))
    {
        qCritical(log) << "CFactory::GetFace is null";
        return false;
    }
    if(!GetDector(szName))
    {
        qCritical(log) << "CFactory::GetDector is null";
        return false;
    }
    if(!GetTracker(szName))
    {
        qCritical(log) << "CFactory::GetTracker is null";
        return false;
    }
    if(!GetLandmarker(szName))
    {
        qCritical(log) << "CFactory::GetLandmarker is null";
        return false;
    }
    if(!GetRecognizer(szName))
    {
        qCritical(log) << "CFactory::GetRecognizer is null";
        return false;
    }
    if(!GetFaceTools(szName))
    {
        qCritical(log) << "CFactory::GetFaceTools is null";
        return false;
    }
    if(!GetDatabase(szName))
    {
        qCritical(log) << "CFactory::GetDatabase is null";
        return false;
    }
    return true;
}

CFace* CFactoryFace::GetFace(const QString &szName)
{
    if(szName.isEmpty())
    {
        if(m_CurrentLib >= 0 && m_CurrentLib < m_Faces.size())
            return m_Faces[m_CurrentLib];
        if(m_bOnlyUserCurrent)
            return nullptr;

        foreach(auto d, m_Faces)
            if(d) return d;
        return nullptr;
    }

    foreach(auto d, m_Faces)
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
        
        foreach(auto d, m_Faces) {
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
        
        foreach(auto d, m_Faces) {
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
        
        foreach(auto d, m_Faces) {
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
        
        foreach(auto d, m_Faces) {
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
        
        foreach(auto d, m_Faces) {
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
        
        foreach(auto d, m_Faces) {
            if(d && d->GetDatabase())
                return d->GetDatabase();
        }
    }
    return GetFace(szName) ? GetFace(szName)->GetDatabase() : nullptr;
}

int CFactoryFace::setModelPath(const QString &szPath)
{
    if(m_bOnlyUserCurrent)
    {
        CFace* pFace = GetFace(); 
        if(!pFace) return -1;
        
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
        return 0;
    }

    foreach(auto pFace, m_Faces)
    {
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

int CFactoryFace::FindPlugins(QDir dir, QStringList filters)
{
    QString fileName;
    if(filters.isEmpty())
    {
#if defined (Q_OS_WINDOWS) || defined(Q_OS_WIN)
        filters << "*PluginFace*.dll";
#else
        filters << "*PluginFace*.so";
#endif
    }
    QStringList files = dir.entryList(filters, QDir::Files | QDir::CaseSensitive);
    if(files.isEmpty())
        qDebug(log) << "Plugin folder is empty:" << dir.absolutePath() << filters;
    else
    {
        //This method is invalid
        //QCoreApplication::addLibraryPath(QDir::cleanPath(dir.absolutePath()));

        QDir::setCurrent(QDir::cleanPath(dir.absolutePath()));

        // This method is valid
        //#if defined(Q_OS_WINDOWS)
        //        QString szPath = QString::fromLocal8Bit(qgetenv("PATH"));
        //        szPath += ";";
        //        szPath += QDir::cleanPath(QDir::cleanPath(dir.absolutePath()));
        //        qputenv("PATH", szPath.toLatin1());
        //#endif
    }
    foreach (fileName, files) {
        //qDebug(log) << "file name:" << fileName;
        QString szPlugins = dir.absoluteFilePath(fileName);
        QPluginLoader loader(szPlugins);
        QObject *plugin = loader.instance();
        if (plugin) {
            CFace* pPlugFace = qobject_cast<CFace*>(plugin);
            if(pPlugFace)
            {
                RegisterFace(pPlugFace);
                qInfo(log) << "Load plugin:" << pPlugFace->GetName()
                               << "; Path:" << szPlugins;
                continue;
            }
            qDebug(log) << "Load plugin: it is not face plugin." << szPlugins;
        }else{
            qCritical(log) << "Load plugin error:" << loader.errorString();
        }
    }

    foreach (fileName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir pluginDir = dir;
        if(pluginDir.cd(fileName))
            FindPlugins(pluginDir, filters);
    }

    return 0;
}

const QString CFactoryFace::Detail() const
{
    QString szDetail;
    foreach(auto f, m_Faces)
    {
        szDetail += f->Detail() + "\n";
    }
    return szDetail;
}
