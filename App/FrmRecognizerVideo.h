#ifndef FRMRECOGNIZERVIDEO_H
#define FRMRECOGNIZERVIDEO_H

#include <QWidget>
#include <QMap>
#include <QMutex>

#include "Face.h"

namespace Ui {
class CFrmRecognizerVideo;
}

class CFrmRecognizerVideo : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRecognizerVideo(QWidget *parent = nullptr);
    virtual ~CFrmRecognizerVideo();

public Q_SLOTS:
    void slotDisplay(const QImage &image);
    void slotRecognized(const QMap<int, QString> &faceInfo);

Q_SIGNALS:
    void sigRecognize(const QImage &image, const QVector<CTracker::strFace> &faces);

private:
    enum STATUS_TYPE
    {
        NORMAL,
        ERROR,
        FAIL,
        SUCCESS
    };
    int SetStatusInformation(const QString &szInfo, int nRet = 0, STATUS_TYPE type = NORMAL);
    
private:
    Ui::CFrmRecognizerVideo *ui;
    QImage m_Image;
    CFace* m_pFace;
    QMap<int, QString> m_FaceInfo;
    QMutex m_Mutex;
};

#endif // FRMRECOGNIZERVIDEO_H
