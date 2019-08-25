#ifndef CFRMDISPLAY_H
#define CFRMDISPLAY_H

#include <QWidget>
#include <QVideoFrame>
#include <QSharedPointer>
#include <QImage>

#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceDatabase.h>

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
    int SetModelPath(const QString &szPath);
    
public Q_SLOTS:
    void slotDisplay(const QVideoFrame &frame);

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    int InitSeeta(const QString &szPath = QString());
    int Detecetor(QImage &image);
    
private:
    Ui::CFrmDisplay *ui;

    QImage m_Image;
    int m_Rotation;

    seeta::ModelSetting::Device m_Device;
    QSharedPointer<seeta::ModelSetting> m_FD_model;
    QSharedPointer<seeta::ModelSetting> m_FL_model;
    
    QSharedPointer<seeta::FaceDetector> m_FD;
    QSharedPointer<seeta::FaceLandmarker> m_FL;
};

#endif // CFRMDISPLAY_H
