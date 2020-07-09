#ifndef CTEST_H
#define CTEST_H

#include <QObject>
#include "FactoryFace.h"

class CFaceTest : public CFace
{
    Q_OBJECT
public:
    explicit CFaceTest(QObject *parent = nullptr);
    virtual ~CFaceTest() override;

    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.Test.CTest")
#endif

    int Initialize(CFactoryFace* pFactoryFace = nullptr) override;
    int Clean(CFactoryFace* pFactoryFace = nullptr) override;
};

#endif // CTEST_H
