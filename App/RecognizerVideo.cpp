#include "RecognizerVideo.h"
#include <QtDebug>
#include <QThread>

CRecognizerVideo::CRecognizerVideo(QObject *parent) : QObject(parent)
{}

void CRecognizerVideo::slotRecognize(const QImage &image)
{
    qDebug() << "CRecognizerVideo::slotRecognize" << QThread::currentThreadId();
}
