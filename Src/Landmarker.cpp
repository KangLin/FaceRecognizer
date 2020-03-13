#include "Landmarker.h"
#include "Log.h"
#include "RabbitCommonDir.h"

CLandmarker::CLandmarker(CFace *pFace, QObject *parent)
    : QObject(parent),
      m_pParameter(nullptr),
      m_pFace(pFace)
{
    m_szModelPath = RabbitCommon::CDir::Instance()->GetDirData(false)
            + QDir::separator() + "model";
    LOG_MODEL_INFO("CParameter", "szPath:%s", m_szModelPath.toStdString().c_str());
#if defined(Q_OS_ANDROID)
    QDir d;
    if(!d.exists(m_szModelPath))
    {
        d.mkpath(m_szModelPath);
        RabbitCommon::CDir::CopyDirectory(
                    RabbitCommon::CDir::Instance()->GetDirData(true)
                                + QDir::separator() + "model",
                    m_szModelPath);
    }
#endif
}

CLandmarker::~CLandmarker()
{}

int CLandmarker::SetParameter(CParameterLandmark *pPara)
{
    if(!pPara)
        return -1;
    
    m_pParameter = pPara;
    bool bCheck = connect(m_pParameter, SIGNAL(destroyed()),
                          this, SLOT(slotParameterDelete()));
    Q_ASSERT(bCheck);
    bCheck = connect(m_pParameter, SIGNAL(sigUpdate()),
                     this, SLOT(slotParameterUpdate()));
    Q_ASSERT(bCheck);
    
    emit m_pParameter->sigUpdate();
    return 0;
}

int CLandmarker::Mark(const QImage &image,
                      const QVector<QRect> &faces,
                      QVector<QVector<QPointF> > &points)
{
    int nRet = 0;
    foreach(auto face, faces)
    {
        QVector<QPointF> p;
        nRet = Mark(image, face, p);
        if(nRet) return nRet;
        points.push_back(p);
    }
    return nRet;
}

void CLandmarker::slotParameterDelete()
{
    m_pParameter = nullptr;
}

void CLandmarker::slotParameterUpdate()
{
    QString szErr;
    int nRet = UpdateParameter(szErr);
    if(nRet)
        LOG_MODEL_ERROR("CLandmarker", "UpdateParameter: %s",
                         szErr.toStdString().c_str());
}

int CLandmarker::setModelPath(const QString &szPath)
{
    m_szModelPath = szPath;
    return 0;
}

QString CLandmarker::getModelPath()
{
    return m_szModelPath;
}
