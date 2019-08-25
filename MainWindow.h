#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>

#include "FrmDisplay.h"
#include "CameraQtCaptureVideoFrame.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionDetector_triggered();
    void on_actionLandmarker_triggered();
    void on_actionRecognizer_triggered();

    void on_actionSet_model_path_triggered();
    
private:
    int CamerOrientation(const QCamera camera);
    int CamerOrientation(const QCameraInfo cameraInfo);
    
private:
    Ui::MainWindow *ui;
    
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    CFrmDisplay m_Display;
    QCamera* m_pCamera;
    
    enum OPERATE{
        DETECETOR,
        LANDMARKERE,
        RECONGNIZER
    };
    OPERATE m_Operate;
};

#endif // MAINWINDOW_H
