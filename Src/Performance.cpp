#include "Performance.h"
#include <QDebug>

CPerformance::CPerformance(const QString &szName, QObject *parent)
    : QObject(parent),
      m_szName(szName)
{
    m_szMessage = szName + ":\n";
    StartTime();
}

CPerformance::~CPerformance()
{
    qDebug().noquote() << m_szMessage;
}

int CPerformance::StartTime()
{
    m_Time = QTime::currentTime();
    return 0;
}

int CPerformance::AddTime(const QString &szFile,
                          int nLine,
                          const QString &szFunction,
                          const QString &szName)
{
    int nUse = m_Time.msecsTo(QTime::currentTime());
    m_szMessage += "  ";
    if(!szName.isEmpty())
        m_szMessage += szName +  ": ";
    else
        m_szMessage += "Use time: ";
    m_szMessage += QString::number(nUse) + "ms"
            + " in " + szFile + "(" + QString::number(nLine) + ")[" + szFunction + "]"
            + "\n";
    m_Time = QTime::currentTime();        
    return 0;
}
