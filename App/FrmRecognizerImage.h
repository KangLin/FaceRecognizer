#ifndef FRMRECOGNIZERIMAGE_H
#define FRMRECOGNIZERIMAGE_H

#include <QWidget>
#include "Face.h"

namespace Ui {
class CFrmRecognizerImage;
}

class CFrmRecognizerImage : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRecognizerImage(QWidget *parent = nullptr);
    ~CFrmRecognizerImage();
    
private slots:
    void on_pbBrower_clicked();
    void on_tvInformation_clicked(const QModelIndex &index);
    
private:
    int RecognizeFace(QImage &image);
    int MarkFace(QImage &image, int nSelect = -1);
    int ShowUI(bool bShow = true);
    enum STATUS_TYPE
    {
        NORMAL,
        ERROR,
        FAIL,
        SUCCESS
    };
    int SetStatusInformation(const QString &szInfo, int nRet = 0, STATUS_TYPE type = NORMAL);
    struct FACE_INFO
    {
        QRect face;
        QVector<QPointF> points;
        qint64 index;
        CDataRegister data;
    };
    QVector<FACE_INFO> m_Info;
    
private:
    Ui::CFrmRecognizerImage *ui;
    CFace* m_pFace;
    QImage m_Image;
};

#endif // FRMRECOGNIZERIMAGE_H
