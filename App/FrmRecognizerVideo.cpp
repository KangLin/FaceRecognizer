#include "FrmRecognizerVideo.h"
#include "ui_FrmRecognizerVideo.h"
#include "Factory.h"
#include "Performance.h"
#include "Log.h"

#include <QPainter>
#include <QDebug>
#include <QMutexLocker>

CFrmRecognizerVideo::CFrmRecognizerVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRecognizerVideo)
{
    ui->setupUi(this);
    SetStatusInformation(tr("Please stand in front of the camera"));
    m_pFace = CFactory::Instance();
    if(!m_pFace)
    {
        throw std::runtime_error("CFrmRegisterVideo consturct allocte memory fail");
    }
}

CFrmRecognizerVideo::~CFrmRecognizerVideo()
{
    qDebug() << "CFrmRecognizerVideo::~CFrmRecognizerVideo()";
    delete ui;
}

int CFrmRecognizerVideo::SetStatusInformation(const QString &szInfo, int nRet, STATUS_TYPE type)
{
    QPalette palette;
    QString szMsg;
    switch (type) {
    case ERROR:
        szMsg = tr("ERROR: ");
        palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
        break;
    case FAIL:
        szMsg = tr("FAIL: ");
        palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
        break;
    case SUCCESS:
        szMsg = tr("SUCCESS: ");
        palette.setColor(QPalette::WindowText, QColor(0, 255, 0));
        break;
    default:
        break;
    }
    szMsg += szInfo;

    ui->lbStatus->setPalette(palette);
    ui->lbStatus->setText(szMsg);
    return nRet;
}

void CFrmRecognizerVideo::slotDisplay(const QImage &image)
{
    if(isHidden() || !m_pFace)
    {
        LOG_MODEL_ERROR("CFrmRecognizerVideo", "isHidden() || !m_pFace");
        return;
    }

    QImage img = image;
    QPainter painter(&img);
    QPen pen(Qt::green);
    pen.setWidth(2);
    painter.setPen(pen);
    PERFORMANCE(CFrmRecognizerVideo)
    QVector<CTracker::strFace> faces;
    m_pFace->GetTracker()->Track(image, faces);
    PERFORMANCE_ADD_TIME(CFrmRecognizerVideo,
                         "Track " + QString::number(faces.size()) + " faces")
    bool bRecognize = false;
    foreach (auto face, faces) {
        QRect f = face.rect;
        painter.drawRect(f.x(), f.y(), f.width(), f.height());
        QMutexLocker locker(&m_Mutex);
        if(m_FaceInfo.end() == m_FaceInfo.find(face.pid))
        {
            bRecognize = true;
            painter.drawText(f.x(), f.y(), QString::number(face.pid));
            m_FaceInfo[face.pid];
        }
        else
            painter.drawText(f.x(), f.y(), QString::number(face.pid)
                             + "-" + m_FaceInfo.find(face.pid).value());
    }
    PERFORMANCE_ADD_TIME(CFrmRecognizerVideo, "MarkFace")
    ui->wgDisplay->slotDisplay(img);
    if(bRecognize)
    {
        QMutexLocker locker(&m_Mutex);
        emit sigRecognize(image, faces);
        PERFORMANCE_ADD_TIME(CFrmRecognizerVideo, "sigRecognize")
    }
    if(faces.size() > 1)
    {
        SetStatusInformation(tr("Please only a person before the camera"));
        return;
    } else if(faces.size() == 0) {
        SetStatusInformation(tr("Please face befor camera"));
        return;
    }

    //TOD: recognizer
    
}

void CFrmRecognizerVideo::slotRecognized(const QMap<int, QString> &faceInfo)
{
    QMutexLocker locker(&m_Mutex);
    m_FaceInfo = faceInfo;
}
