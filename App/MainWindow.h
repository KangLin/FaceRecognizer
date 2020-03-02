#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QMediaPlayer>

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
    void slotCameraChanged(int index);
    void slotCameraOrientation(QAction* pAction);
    void on_actionStart_triggered();
    void on_actionRegisterImage_triggered();
    void on_actionRegisterVideo_triggered();
    void on_actionRegisterImage_directory_triggered();
    void on_actionRecognizerImage_triggered();
    void on_actionRecognizerVideo_triggered();
    void on_actionSet_model_path_triggered();
    void on_actionAbout_A_triggered();
    void on_actionUpdate_U_triggered();
    void on_actionDisplay_triggered();
    void on_actionFile_triggered();
    void on_actionExit_triggered();
    void on_actionOpen_log_file_triggered();
    void on_actionAiLibraries_triggered(QAction *action);

private:
    int CamerOrientation(int index);
    int InitCamerOrientation();

private:
    Ui::MainWindow *ui;
    
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    QCamera* m_pCamera;
    QMediaPlayer m_Player;

};

#endif // MAINWINDOW_H
