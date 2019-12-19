#include "Factory.h"
#ifdef HAVE_SEETA_FACE
    #include "Seeta/FaceSeeta.h"
#endif
#include "ParameterFactory.h"

CFactory::CFactory(QObject *parent)
{
    Q_UNUSED(parent)
}

CFactory::~CFactory()
{
}

CFace* CFactory::Instance()
{
    static CFace* pFace = nullptr;
#ifdef HAVE_SEETA_FACE
    if(!pFace)
        pFace = new CFaceSeeta();
#endif
    return pFace;
}
