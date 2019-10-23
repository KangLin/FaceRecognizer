#include "Face.h"

CFace::CFace(QObject *parent) : QObject(parent)
{
    m_pDatabase = new CDatabase();
}

CFace::~CFace()
{
    if(m_pDatabase)
        delete m_pDatabase;
}

CDatabase* CFace::GetDatabase()
{
    return m_pDatabase;
}
