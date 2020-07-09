#include "Test.h"
#include "Log.h"

CFaceTest::CFaceTest(QObject *parent) : CFace(parent)
{}

CFaceTest::~CFaceTest()
{}

int CFaceTest::Initialize(CFactoryFace *pFactoryFace)
{
    int nRet = 0;
    LOG_MODEL_INFO("Test", "CFaceTest::Initialize");
    nRet = pFactoryFace->RegisterFace("Test", this, tr("Test"));
    return nRet;
}

int CFaceTest::Clean(CFactoryFace* pFactoryFace)
{
    int nRet = 0;
    nRet = pFactoryFace->RemoveFace("Test");
    return nRet;
}
