#ifndef CMANAGEREGISTERVIDEO_H
#define CMANAGEREGISTERVIDEO_H

#include <QWidget>
#include <QResizeEvent>

#include "FrmRegisterVideo.h"
#include "FrmRegisterImage.h"
#include "ParameterRegisterImage.h"

class CManageRegisterVideo : public QWidget
{
    Q_OBJECT
public:
    explicit CManageRegisterVideo(QWidget *parent = nullptr);

public Q_SLOTS:
    void slotRigister(const CParameterRegisterImage &para);
    void slotRegisterFinish();

Q_SIGNALS:
    void sigCaptureFrame(const QImage &image);
    
private:
    QSharedPointer<CFrmRegisterVideo> m_RegisterVideo;
    QSharedPointer<CFrmRegisterImage> m_RegisterImage;

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // CMANAGEREGISTERVIDEO_H
