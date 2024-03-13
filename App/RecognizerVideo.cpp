/**
  @author: Kang Lin<kl222@126.com>
  */

#include "RecognizerVideo.h"
#include "Performance.h"

#include <QThread>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "App.Recognizer.Video")

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
            qCritical(log) << "points is empty";
            continue;
        }
        qint64 index = pFace->GetRecognizer()->Query(image, face.rect);
        PERFORMANCE_ADD_TIME(CRecognizerVideo, "Query")
        if(-1 == index)
        {
            qCritical(log) << "pid[" << face.pid << "] don't query";
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
