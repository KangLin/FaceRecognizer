/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Log.cpp

Abstract:

    This file contains log implement.
 */

#include "Log.h"
#include <string>
#include <stdarg.h>
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QDesktopServices>

#define LOG_BUFFER_LENGTH 1024

CLog::CLog() : QObject()
{
    m_pBuffer = new char[LOG_BUFFER_LENGTH];
}

CLog::~CLog()
{
    if(m_pBuffer) delete []m_pBuffer;
}

CLog* CLog::Instance()
{
    static CLog* p = nullptr;
    if(!p)
        p = new CLog;
    return p;
}

int CLog::Log(const char *pszFile, int nLine, int nLevel,
              const char* pszModelName, const char *pFormatString, ...)
{
    if(!m_pBuffer) return -1;
    QString szTemp = pszFile;
    szTemp += "(";
    sprintf(m_pBuffer, "%d", nLine);
    szTemp += m_pBuffer;
    szTemp += "):";
    switch(nLevel)
    {
    case LM_DEBUG:
        szTemp += "DEBUG";
        break;
    case LM_ERROR:
        szTemp += "ERROR";
        break;
    case LM_INFO:
        szTemp += "INFO";
        break;
    case LM_WARNING:
        szTemp = "WARNING";
        break;
    }
    szTemp += ":";
    szTemp += pszModelName;
    szTemp += ":";

    va_list args;
    va_start (args, pFormatString);
    int nRet = vsnprintf(m_pBuffer, LOG_BUFFER_LENGTH, pFormatString, args);
    va_end (args);
    if(nRet < 0 || nRet >= LOG_BUFFER_LENGTH)
    {
        LOG_MODEL_ERROR("Global",
                        "vsprintf buf is short, %d > %d. Truncated it:%d",
                        nRet > LOG_BUFFER_LENGTH, LOG_BUFFER_LENGTH);
        m_pBuffer[LOG_BUFFER_LENGTH - 1] = 0;
        //return nRet;
    }
    szTemp += m_pBuffer;

    Log(szTemp);

    return 0;
}

int CLog::Log(const QString &szLog)
{
    qCritical() << szLog;
    if(!m_szFile.isEmpty())
    {
        QFile f(m_szFile);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "open file fail: "
                     << m_szFile
                     << "["
                     << f.error()
                     << "]"
                     << f.errorString();
            return 0;
        }
        QTextStream out(&f);  
        out << szLog << endl;
        f.close();
    }
    
    emit sigLog(szLog + "\n");
    return 0;
}

int CLog::SetSaveFile(const QString &szFile)
{
    m_szFile = szFile;
    return 0;
}

QString CLog::GetSaveFile()
{
    return m_szFile;
}

int CLog::OpneFile()
{
    if(QDesktopServices::openUrl(QUrl::fromLocalFile(m_szFile)))
        return 0;
    return -1;
}
