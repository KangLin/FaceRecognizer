#ifndef FRMRECOGNIZERVIDEO_H
#define FRMRECOGNIZERVIDEO_H

#include <QWidget>
#include "Face.h"

namespace Ui {
class CFrmRecognizerVideo;
}

class CFrmRecognizerVideo : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRecognizerVideo(QWidget *parent = nullptr);
    ~CFrmRecognizerVideo();

public Q_SLOTS:
    void slotDisplay(const QImage &image);

Q_SIGNALS:
    void sigRecognizer(const QImage &image);

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
};

#endif // FRMRECOGNIZERVIDEO_H
