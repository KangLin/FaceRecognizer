#include "DataRegister.h"

CDataRegister::CDataRegister(QObject *parent) : QObject(parent)
{}

qint64 CDataRegister::getIndex()
{
    return m_nIndex;
}

int CDataRegister::setIndex(qint64 index)
{
    m_nIndex = index;
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
