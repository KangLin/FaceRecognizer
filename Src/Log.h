/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Log.h

Abstract:

    This file contains log define.
 */

#ifndef CLOG_H
#define CLOG_H
#include <QObject>

#include "facerecognizer_export.h"

class FACERECOGNIZER_EXPORT CLog : public QObject
{
    Q_OBJECT

public:
    CLog();

    static CLog* Instance();

    int SetSaveFile(const QString &szFile);
    QString GetSaveFile();
public Q_SLOTS:
    int OpneFile();

public:
    /**
     * @brief 日志  
     * @param pszFile:打印日志处文件名  
     * @param nLine:打印日志处行号  
     * @param nLevel:打印日志错误级别  
     * @param pszModelName:打印日志的模块范围  
     * @param pFormatString:格式化字符串  
     * @return 
     */
    int Log(const char *pszFile, int nLine, int nLevel,
            const char* pszModelName, const char *pFormatString, ...);
    int Log(const QString &szLog);

signals:
    void sigLog(const QString &szLog);

private:
    QString m_szFile;
};

#define LM_DEBUG 0
#define LM_INFO 1
#define LM_WARNING 2
#define LM_ERROR 3

#ifdef DEBUG
#define LOG_ERROR(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_DEBUG, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_DEBUG, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_INFO(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)

#else

#define LOG_ERROR(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) 
#define LOG_WARNING(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...) 
#define LOG_MODEL_INFO(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)

#endif//#ifdef DEBUG
#endif // CLOG_H
/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Log.h

Abstract:

    This file contains log define.
 */

#ifndef CLOG_H
#define CLOG_H

class CLog
{
public:
    CLog();

    static CLog* Instance();
    
    /**
     * @brief 日志  
     * @param pszFile:打印日志处文件名  
     * @param nLine:打印日志处行号  
     * @param nLevel:打印日志错误级别  
     * @param pszModelName:打印日志的模块范围  
     * @param pFormatString:格式化字符串  
     * @return 
     */
    int Log(const char *pszFile, int nLine, int nLevel,
            const char* pszModelName, const char *pFormatString, ...);
};


#define LM_DEBUG 0
#define LM_INFO 1
#define LM_WARNING 2
#define LM_ERROR 3

#ifdef DEBUG
#define LOG_ERROR(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_DEBUG, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_DEBUG, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_INFO(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)

#else

#define LOG_ERROR(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)
#define LOG_WARNING(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...)
#define LOG_MODEL_INFO(model, fmt, ...) CLog::Instance()->Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)

#endif//#ifdef DEBUG
#endif // CLOG_H
