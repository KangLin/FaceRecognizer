#include "RecognizerVideo.h"
#include "Log.h"
#include "Performance.h"

#include <QtDebug>
#include <QThread>

static int gIdQMapIntQString = qRegisterMetaType<QMap<int,QString> >();
CRecognizerVideo::CRecognizerVideo(QObject *parent) : QObject(parent)
{}

void CRecognizerVideo::slotRecognize(const QImage &image, const QVector<CTracker::strFace> &faces)
{
    if(image.isNull() || faces.isEmpty()) return;

    CFactoryFace *pFace = CFactoryFace::Instance();
    if(!pFace->bIsValid()) return;

    PERFORMANCE(CRecognizerVideo)
    QMap<int, QString> faceInfo;
    foreach (auto face, faces) {
        QVector<QPointF> points;
        pFace->GetLandmarker()->Mark(image, face.rect, points);
        PERFORMANCE_ADD_TIME(CRecognizerVideo,
                             "Mark points:" + QString::number(points.size()))
        if(points.isEmpty())
        {
            LOG_MODEL_ERROR("CRecognizerVideo", "points is empty");
            continue;
        }
        qint64 index = pFace->GetRecognizer()->Query(image, face.rect);
        PERFORMANCE_ADD_TIME(CRecognizerVideo, "Query")
        if(-1 == index)
        {
            LOG_MODEL_ERROR("CRecognizerVideo", "pid[%d] don't query", face.pid);
            continue;
        }
        CDataRegister data;
        int nRet = pFace->GetDatabase()->GetTableRegister()->GetRegisterInfo(index, &data);
        PERFORMANCE_ADD_TIME(CRecognizerVideo, "GetRegisterInfo")
        if(nRet)
            continue;
        faceInfo.insert(face.pid, data.getName());
    }
    if(!faceInfo.isEmpty())
    {
        emit sigRecognized(faceInfo);
        //qDebug() << "emit sigRecognized(faceInfo); end";
    }
}
