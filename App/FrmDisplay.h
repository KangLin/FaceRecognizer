#ifndef CFRMDISPLAY_H
#define CFRMDISPLAY_H

#include <QWidget>
#include <QImage>
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
    static void YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height);
    
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
