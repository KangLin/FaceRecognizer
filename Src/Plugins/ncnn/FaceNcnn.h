#ifndef CFACENCNN_H
#define CFACENCNN_H

#include "Face.h"

class CFaceNcnn : public CFace
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    Q_PROPERTY(DectorAlgorithm dectorAlgorithm READ getDectorAlgorithm WRITE setDectorAlgorithm)

public:
    explicit CFaceNcnn(QObject *parent = nullptr);
    virtual ~CFaceNcnn() override;
    
    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.ncnn.CFaceNcnn")
#endif

    virtual int Initialize() override;
    virtual QString GetName() override;
    
    enum DectorAlgorithm
    {
        NOT,
        RETINA
    };
    Q_ENUM(DectorAlgorithm)
    DectorAlgorithm getDectorAlgorithm();
    int setDectorAlgorithm(DectorAlgorithm algorithm = RETINA);
private:
    DectorAlgorithm m_DectorAlgorithm;
};

#endif // CFACENCNN_H
