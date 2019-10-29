#include "RecognizerVideo.h"
#include "Log.h"

#include <QtDebug>
#include <QThread>

static int gIdQMapIntQString = qRegisterMetaType<QMap<int,QString> >();
CRecognizerVideo::CRecognizerVideo(QObject *parent) : QObject(parent)
{}

void CRecognizerVideo::slotRecognize(const QImage &image, const QVector<CTracker::strFace> &faces)
{
    if(image.isNull()) return;
    CFace *pFace = CFactory::Instance();
    if(!pFace) return;
    
    QMap<int, QString> faceInfo;
    foreach (auto face, faces) {
        QVector<QPointF> points;
        pFace->GetLandmarker()->Mark(image, face.rect, points);
        if(points.isEmpty())
        {
            LOG_MODEL_ERROR("CRecognizerVideo", "points is empty");
            continue;
        }
        qint64 index = pFace->GetRecognizer()->Query(image, points);
        if(-1 == index)
        {
            LOG_MODEL_ERROR("CRecognizerVideo", "pid[%d] don't query", face.pid);
            continue;
        }
        CDataRegister data;
        if(pFace->GetDatabase()->GetTableRegister()->GetRegisterInfo(index, &data))
            continue;
        faceInfo.insert(face.pid, data.getName());
    }
    if(!faceInfo.isEmpty())
    {
        emit sigRecognized(faceInfo);
    }
}
