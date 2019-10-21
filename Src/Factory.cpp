#include "Factory.h"
#include "Seeta/FaceSeeta.h"
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
#ifdef HAVE_SEETA_FACE
    return new CFaceSeeta();
#else
    return nullptr;
#endif
}
