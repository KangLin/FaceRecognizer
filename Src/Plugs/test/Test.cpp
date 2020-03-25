#include "Test.h"
#include "Log.h"

CTest::CTest(QObject *parent) : CFace(parent)
{}

CTest::~CTest()
{}

int CTest::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    LOG_MODEL_INFO("Test", "CTest::Initialize");
    nRet = pFactoryFace->RegisterFace("Face", this, tr("Seeta"));
    return nRet;
}

int CTest::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("Face");
    return nRet;
}
