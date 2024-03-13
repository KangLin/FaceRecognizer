
#ifndef CFACE_H
#define CFACE_H

#include <QObject>
#include <QMetaClassInfo>

#include "facerecognizer_export.h"

#include "Detector.h"
#include "Tracker.h"
#include "Landmarker.h"
#include "Recognizer.h"
#include "FaceTools.h"
#include "database/Database.h"

/*!
 * \~english Face recognition plug-in interface. Implemented by plugins.
 *
 * \~chinese
 * \brief 人脸识别插件接口。由插件实现。
 * 
 * \~
 * \see CFactoryFace
 * \ingroup API_INTERNAL
 */
class FACERECOGNIZER_EXPORT CFace : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

    Q_PROPERTY(QString name READ GetName)
    Q_PROPERTY(QString descritp READ GetDescript)
    Q_PROPERTY(int level READ GetLevel)

public:
    explicit CFace(QObject *parent = nullptr);
    virtual ~CFace();

    virtual int Initialize();
    virtual int Clean();
    virtual QString GetName();
    virtual QString GetDescript();
    enum LEVEL
    {
        HEIGHT = 1,
        NORMAL = 0,
        LOWER = -1
    };
    virtual int GetLevel();

    bool IsValid();

    virtual CDetector* GetDector();
    virtual CTracker* GetTracker();
    virtual CLandmarker* GetLandmarker();
    virtual CRecognizer* GetRecognizer();
    virtual CFaceTools* GetFaceTools();
    virtual CDatabase* GetDatabase();

protected:
    CDetector* m_pDetector;
    CTracker* m_pTracker;
    CLandmarker* m_pLandmarker;
    CRecognizer* m_pRecognizer;
    CFaceTools* m_pFaceTools;

private:
    CDatabase* m_pDatabase;
};

#define FACE_IID "KangLinStudio.Rabbit.FaceRecognizer.Plugs.Interface.CFace"
Q_DECLARE_INTERFACE(CFace, FACE_IID)

#endif // CFACE_H
