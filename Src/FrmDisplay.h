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
    
public Q_SLOTS:
    void slotDisplay(const QImage &frame);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::CFrmDisplay *ui;

    QImage m_Image;
};

#endif // CFRMDISPLAY_H
