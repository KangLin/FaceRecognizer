#include "ConverFormat.h"
#include "ImageTool.h"

CConverFormat::CConverFormat(QObject *parent) : QObject(parent)
{}

int CConverFormat::Initialize(CImageTool *pTool)
{
    return pTool->Register(this, getName());
}

int CConverFormat::Clean(CImageTool *pTool)
{
    return pTool->Remove(getName());
}

QString CConverFormat::getName()
{
    return this->metaObject()->className();
}
