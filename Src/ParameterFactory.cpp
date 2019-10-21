#include <QSettings>
#include <QDebug>

#include "ParameterFactory.h"
#include "RabbitCommonDir.h"

CParameterFactory::CParameterFactory(QObject *parent) : QObject(parent)
{
    //注意生成顺序  
    CParameter* pPara = new CParameterDetector(this);
    m_pParameter.push_back(pPara);
    pPara = new CParameterLandmark(this);
    m_pParameter.push_back(pPara);
    pPara = new CParameterRecognizer(this);
    m_pParameter.push_back(pPara);
    
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szFile = set.value("ModuleDir").toString();
    SetModelPath(szFile);
    qDebug() << "Model files:" << szFile;
}

CParameterFactory::~CParameterFactory()
{
    foreach (CParameter* p, m_pParameter) {
        if(p)
            delete p;
    }
}

CParameterFactory* CParameterFactory::Instance()
{
    static CParameterFactory* pThis = nullptr;
    if(!pThis)
        pThis = new CParameterFactory();
    return pThis;
}

CParameterDetector* CParameterFactory::GetParameterDetector()
{
    return dynamic_cast<CParameterDetector*>(m_pParameter[DETECTORY]);
}

CParameterLandmark* CParameterFactory::GetParameterLandmark()
{
    return dynamic_cast<CParameterLandmark*>(m_pParameter[LANDMARKER]);
}

CParameterRecognizer* CParameterFactory::GetParameterRecognizer()
{
    return dynamic_cast<CParameterRecognizer*>(m_pParameter[RECOGNIZER]);
}

int CParameterFactory::SetModelPath(const QString &szPath)
{
    foreach(CParameter* p, m_pParameter)
    {
        if(p)
            p->SetModelPath(szPath);
    }
    return 0;
}

int CParameterFactory::SetDevice(CParameter::DEVICE device)
{
    foreach(CParameter* p, m_pParameter)
    {
        if(p)
            p->SetDevice(device);
    }
    return 0;
}
