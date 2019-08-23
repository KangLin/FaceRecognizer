#ifndef CFRMDISPLAY_H
#define CFRMDISPLAY_H

#include <QWidget>
#include <QVideoFrame>

namespace Ui {
class CFrmDisplay;
}

class CFrmDisplay : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmDisplay(QWidget *parent = nullptr);
    ~CFrmDisplay();
    
    int SetCameraAngle(int rotation);
    
public Q_SLOTS:
    void slotDisplay(const QVideoFrame &frame);

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    Ui::CFrmDisplay *ui;
    
    QVideoFrame m_VideoFrame;
    int m_Rotation;

};

#endif // CFRMDISPLAY_H
