#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FrmDisplay.h"
#include <QCamera>
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
    
private:
    int CamerOrientation(const QCamera camera);
    int CamerOrientation(const QCameraInfo cameraInfo);
    
private:
    Ui::MainWindow *ui;
    
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    CFrmDisplay m_Display;
    QCamera* m_pCamera;
};

#endif // MAINWINDOW_H
