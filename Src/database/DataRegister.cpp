#include "DataRegister.h"

CDataRegister::CDataRegister(QObject *parent) : QObject(parent)
{}

CDataRegister::CDataRegister(const CDataRegister &other)
{
    m_nIdx = other.m_nIdx;
    m_nNo = other.m_nNo;
    m_szName = other.m_szName;
}

CDataRegister& CDataRegister::operator=(const CDataRegister &other)
{
    m_nIdx = other.m_nIdx;
    m_nNo = other.m_nNo;
    m_szName = other.m_szName;
    return *this;
}

qint64 CDataRegister::getIdx()
{
    return m_nIdx;
}

int CDataRegister::setIdx(qint64 idx)
{
    m_nIdx = idx;
    return 0;
}

qint64 CDataRegister::getNo()
{
    return m_nNo;
}

int CDataRegister::setNo(qint64 nNo)
{
    m_nNo = nNo;
    return 0;
}

QString CDataRegister::getName()
{
    return m_szName;
}

int CDataRegister::setName(const QString &szName)
{
    m_szName = szName;
    return 0;
}
