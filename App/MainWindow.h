/**
  @author: Kang Lin<kl222@126.com>
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QMediaPlayer>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    #include <QMediaCaptureSession>
#endif
#include "CameraQtCaptureVideoFrame.h"
#include "FrmPara.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

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
    void on_actionAiLibraries_triggered(QAction *action);
    void on_actionStyle_triggered();

    void slotParaDock_triggered(bool checked);
    void slotScreenOrientationChanged(Qt::ScreenOrientation orientation);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    void slotPlayError(QMediaPlayer::Error error, const QString &errorString);
    void slotMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void slotPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void slotPositionChanged(qint64 position);
#else
    void slotPlayError(QMediaPlayer::Error error);
#endif

private:
    int CamerOrientation(int index);
    int InitCamerOrientation();
    int createDockPerameters();
    int setModelPath(const QString &szPath);

private:
    Ui::MainWindow *ui;

    CCameraQtCaptureVideoFrame m_CaptureFrame;
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    QMediaCaptureSession m_CaptureSession;
#endif
    QCamera* m_pCamera;
    QMediaPlayer m_Player;
    QSharedPointer<CFrmPara> m_Paramter;
};

#endif // MAINWINDOW_H
