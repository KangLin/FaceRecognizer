/**
  @brief: The form is display QImage
  @author: Kang Lin<kl222@126.com>
  */

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
    virtual ~CFrmDisplay();
    
public Q_SLOTS:
    void slotDisplay(const QImage &image);
    int slotSetAspectRatio(bool bAspectRation = true);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::CFrmDisplay *ui;

    QImage m_Image;
    bool m_bAspectRation;
};

#endif // CFRMDISPLAY_H
