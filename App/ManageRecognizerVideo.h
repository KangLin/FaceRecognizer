#ifndef MANAGERECOGNIZERVIDEO_H
#define MANAGERECOGNIZERVIDEO_H

#include <QWidget>
#include <QResizeEvent>

#include "FrmRecognizerVideo.h"
#include "RecognizerThread.h"

namespace Ui {
class CManageRecognizerVideo;
}

class CManageRecognizerVideo : public QWidget
{
    Q_OBJECT
    
public:
    explicit CManageRecognizerVideo(QWidget *parent = nullptr);
    virtual ~CManageRecognizerVideo() override;

public Q_SLOTS:
    void slotRecognizer(const QImage &image);
    void slotRecognizerFinish();

Q_SIGNALS:
    void sigCaptureFrame(const QImage &image);

private:
    Ui::CManageRecognizerVideo *ui;
    CFrmRecognizerVideo *m_pFrmRecognizerVideo;
    CRecognizerThread *m_pThread;

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // MANAGERECOGNIZERVIDEO_H
