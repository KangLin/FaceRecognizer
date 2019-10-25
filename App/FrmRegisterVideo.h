#ifndef FRMREGISTERVIDEO_H
#define FRMREGISTERVIDEO_H

#include <QWidget>
#include <Face.h>

namespace Ui {
class CFrmRegisterVideo;
}

class CFrmRegisterVideo : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRegisterVideo(QWidget *parent = nullptr);
    ~CFrmRegisterVideo();
    
public Q_SLOTS:
    void slotDisplay(const QImage &image);
    
private slots:
    void on_pbRegister_clicked();
    
private:
    enum STATUS_TYPE
    {
        NORMAL,
        ERROR,
        FAIL,
        SUCCESS
    };
    int SetStatusInformation(const QString &szInfo, int nRet = 0, STATUS_TYPE type = NORMAL);
    int Check();
    int MarkFace(QPainter &painter, const QVector<QRect> faces);
    
private:
    Ui::CFrmRegisterVideo *ui;
    QImage m_Image;
    CFace* m_pFace;
    QRect m_box;
    int m_nImageWidth, m_nImageHeight;
};

#endif // FRMREGISTERVIDEO_H
