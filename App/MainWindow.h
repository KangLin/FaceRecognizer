#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_actionRecognizer_triggered();
    void on_actionRegister_triggered();
    void on_actionSet_model_path_triggered();
    void on_actionAbout_A_triggered();
    void on_actionUpdate_U_triggered();
    void on_actionCamera_triggered();

    void slotCameraChanged(int index);

private:
    int CamerOrientation(int index);

private:
    Ui::MainWindow *ui;
    
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    QCamera* m_pCamera;
    
    QString m_szModelFile;
};

#endif // MAINWINDOW_H
