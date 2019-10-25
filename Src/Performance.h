#ifndef CPERFORMANCE_H
#define CPERFORMANCE_H

#include <QObject>
#include <QTime>
#include "ParameterRecognizer.h"

class FACERECOGNIZER_EXPORT CPerformance : public QObject
{
    Q_OBJECT
public:
    explicit CPerformance(const QString &szName = QString(),
                          QObject *parent = nullptr);
    virtual ~CPerformance();
    
    int StartTime();
    int AddTime(const QString &szName = QString());
    
private:
    QString m_szName;
    QString m_szMessage;
    QTime m_Time;
};

#ifdef HAVE_PERFORMANCE
    #define PERFORMANCE(name) \
        CPerformance object##name (#name);
    #define PERFORMANCE_START(name) object##name.StartTime();
    #define PERFORMANCE_ADD_TIME(name, message) object##name.AddTime(message);
#else
    #define PERFORMANCE(name)
    #define PERFORMANCE_START(name)
    #define PERFORMANCE_ADD_TIME(name, message)
#endif
#endif // CPERFORMANCE_H
