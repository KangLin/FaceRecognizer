#include "PlugsManager.h"
#include <QPluginLoader>

#include "Log.h"
#include "Face.h"
#include "FactoryFace.h"
#include "RabbitCommonDir.h"
#include "ImageTool.h"

CPlugsManager::CPlugsManager(QObject *parent) : QObject(parent)
{}

CPlugsManager::~CPlugsManager()
{}

CPlugsManager* CPlugsManager::Instance()
{
    static CPlugsManager* p = nullptr;
    if(!p)
        p = new CPlugsManager();
    return p;
}

int CPlugsManager::LoadPlugs()
{
    int nRet = 0;
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        CFace* pPlugFace = qobject_cast<CFace*>(plugin);
        if(pPlugFace)
        {
            pPlugFace->Initialize(CFactoryFace::Instance());
            continue;
        }
    }
#ifdef RABBITCOMMON
    //LOG_MODEL_INFO("FactoryFace", "Plugs dir:%s", RabbitCommon::CDir::Instance()->GetDirPlugs().toStdString().c_str());
    nRet = FindPlugins(RabbitCommon::CDir::Instance()->GetDirPlugs());
#endif
    return nRet;
}

int CPlugsManager::FindPlugins(QDir dir)
{
    QString szPath = dir.path();
    QString fileName;
    QStringList filters;
#if defined (Q_OS_WINDOWS)
    filters << "*Plug*.dll";
#else
    filters << "*Plug*.so";
#endif
    QStringList files = dir.entryList(filters, QDir::Files | QDir::CaseSensitive);
    foreach (fileName, files) {
        //LOG_MODEL_INFO("CFactoryFace", "file name:%s", fileName.toStdString().c_str());
        QString szPlugins = dir.absoluteFilePath(fileName);
        QPluginLoader loader(szPlugins);
        QObject *plugin = loader.instance();
        if (plugin) {
            CFace* pPlugFace = qobject_cast<CFace*>(plugin);
            if(pPlugFace)
            {
                pPlugFace->Initialize(CFactoryFace::Instance());
                continue;
            }

            CConverFormat* pCF = qobject_cast<CConverFormat*>(plugin);
            if(pCF)
            {
                m_ConverFormat[pCF->getName()] = pCF;
                continue;
            }
        }else{
            LOG_MODEL_ERROR("CFactoryFace", "load plugin error:%s",
                            loader.errorString().toStdString().c_str());
        }
    }

    foreach (fileName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir pluginDir = dir;
        if(pluginDir.cd(fileName))
            FindPlugins(pluginDir);
    }

    return 0;
}

CConverFormat* CPlugsManager::GetConverFormat(const QString &szName)
{
    QMap<QString, CConverFormat*>::iterator it = m_ConverFormat.find(szName);
    if(m_ConverFormat.end() != it)
        return it.value();

    if(szName.isEmpty() && !m_ConverFormat.isEmpty())
        return m_ConverFormat.begin().value();

    return nullptr;
}
