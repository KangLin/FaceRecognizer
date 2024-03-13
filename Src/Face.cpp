#include "Face.h"

#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "Face")

CFace::CFace(QObject *parent) : QObject(parent),
    m_pDetector(nullptr),
    m_pTracker(nullptr),
    m_pLandmarker(nullptr),
    m_pRecognizer(nullptr),
    m_pFaceTools(nullptr),
    m_pDatabase(nullptr)
{
    try {
        m_pDatabase = new CDatabase();
    } catch (...) {
        qCritical(log) << "new CDatabase fail";
    }
}

CFace::~CFace()
{
    Clean();

    if(m_pDatabase)
        delete m_pDatabase;
}

int CFace::Initialize()
{
    return 0;
}

int CFace::Clean()
{
    qDebug(log) << "Clean" << this->metaObject()->className();

    if(m_pDetector)
    {   
        delete m_pDetector;
        m_pDetector = nullptr;
    }
    if(m_pTracker)
    {
        delete m_pTracker;
        m_pTracker = nullptr;
    }
    if(m_pLandmarker)
    {
        delete m_pLandmarker;
        m_pLandmarker = nullptr;
    }
    if(m_pRecognizer)
    {
        delete m_pRecognizer;
        m_pRecognizer = nullptr;
    }
    if(m_pFaceTools)
    {
        delete m_pFaceTools;
        m_pFaceTools = nullptr;
    }

    return 0;
}

QString CFace::GetName()
{
    return this->metaObject()->className();
}

QString CFace::GetDescript()
{
    return GetName();
}

int CFace::GetLevel()
{
    return LOWER;
}

bool CFace::IsValid()
{
    if(!GetDector())
    {
        qCritical(log) << "CFace::GetDector is null";
        return false;
    }
    if(!GetTracker())
    {
        qCritical(log) << "CFace::GetTracker is null";
        return false;
    }
    if(!GetLandmarker())
    {
        qCritical(log) << "CFace::GetLandmarker is null";
        return false;
    }
    if(!GetRecognizer())
    {
        qCritical(log) << "CFace::GetRecognizer is null";
        return false;
    }
    if(!GetFaceTools())
    {
        qCritical(log) << "CFace::GetFaceTools is null";
        return false;
    }
    if(!GetDatabase())
    {
        qCritical(log) << "CFace::GetDatabase is null";
        return false;
    }
    return true;
}

CDetector* CFace::GetDector()
{
    return m_pDetector;
}

CLandmarker* CFace::GetLandmarker()
{
    return m_pLandmarker;
}

CRecognizer* CFace::GetRecognizer()
{
    return m_pRecognizer;
}

CTracker* CFace::GetTracker()
{
    return m_pTracker;
}

CFaceTools* CFace::GetFaceTools()
{
    return m_pFaceTools;
}

CDatabase* CFace::GetDatabase()
{
    return m_pDatabase;
}
