#ifndef CFRMDISPLAY_H
#define CFRMDISPLAY_H

#include <QWidget>
#include <QImage>
#include <QVideoFrame>

#include "facerecognizer_export.h"

namespace Ui {
class CFrmDisplay;
}

class FACERECOGNIZER_EXPORT CFrmDisplay : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmDisplay(QWidget *parent = nullptr);
    ~CFrmDisplay();
    
    int SetCameraAngle(int rotation);
    
public Q_SLOTS:
    void slotDisplay(const QImage &frame);
    virtual void slotDisplay(const QVideoFrame &frame);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::CFrmDisplay *ui;

    QImage m_Image;
    QVideoFrame m_Frame;
    
    int m_Rotation;
};

#endif // CFRMDISPLAY_H
