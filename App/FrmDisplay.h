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

    int SetAspectRatio(bool bAspectRation = true);
    
public Q_SLOTS:
    void slotDisplay(const QImage &image);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::CFrmDisplay *ui;

    QImage m_Image;
    bool m_bAspectRation;
};

#endif // CFRMDISPLAY_H
