/**
  @author: Kang Lin<kl222@126.com>
  */

#include "ParameterRegisterImage.h"
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "App.Manage.Recognizer.Image")
CParameterRegisterImage::CParameterRegisterImage(QObject *parent) : QObject(parent)
{}

CParameterRegisterImage::CParameterRegisterImage(const CParameterRegisterImage &other)
{
    m_No = other.m_No;
    m_szName = other.m_szName;
    m_Image = other.m_Image;
}

int CParameterRegisterImage::SetNo(qint64 no)
{
    m_No = no;
    return 0;
}

qint64 CParameterRegisterImage::GetNo()
{
    return m_No;
}

int CParameterRegisterImage::SetName(const QString &szName)
{
    m_szName = szName;
    return 0;
}

QString CParameterRegisterImage::GetName()
{
    return m_szName;
}

int CParameterRegisterImage::SetImage(const QImage &image)
{
    m_Image = image;
    return 0;
}

QImage CParameterRegisterImage::GetImage()
{
    return m_Image;
}
