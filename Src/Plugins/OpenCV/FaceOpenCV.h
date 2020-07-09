#ifndef FACEOPENCV_H
#define FACEOPENCV_H

#include "Face.h"

class CFaceOpenCV : public CFace
{
    Q_OBJECT
    Q_PROPERTY(Algorithm algorithm READ getAlgorithm WRITE setAlgorithm)

public:
    explicit CFaceOpenCV(QObject *parent = nullptr);
    
    Q_INTERFACES(CFace)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.OpenCV.CFaceOpenCV")
#endif

    virtual int Initialize() override; 
    QString GetName() override;
    
    enum Algorithm
    {
        OPENCV,
        DNN,
        NOT
    };
    Q_ENUM(Algorithm)
    Algorithm getAlgorithm();
    int setAlgorithm(Algorithm algorithm = OPENCV);
private:
    Algorithm m_Algorithm;
};

#endif // FACEOPENCV_H
