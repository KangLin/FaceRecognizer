#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCameraInfo>
#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCamera(nullptr)
{
    ui->setupUi(this);
    
    this->setCentralWidget(&m_Display);
    
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << "Camer name:" << cameraInfo.deviceName();
    }
    
    if(QCameraInfo::availableCameras().size() > 0)
    {
        m_pCamera = new QCamera(QCameraInfo::availableCameras().at(0));
        m_pCamera->setViewfinder(&m_CaptureFrame);
        
        bool check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QVideoFrame &)),
                             &m_Display, SLOT(slotDisplay(const QVideoFrame &)));
        Q_ASSERT(check);
#if defined (Q_OS_ANDROID)
        m_Display.SetCameraAngle(CamerOrientation(QCameraInfo::availableCameras().at(0)));
#else           
        m_Display.SetCameraAngle(180);
#endif
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionStart_triggered()
{
    if(m_pCamera)
        m_pCamera->start();
}

void MainWindow::on_actionStop_triggered()
{
    if(m_pCamera)
        m_pCamera->stop();
}

int MainWindow::CamerOrientation(const QCameraInfo cameraInfo)
{
    // Get the current display orientation
    const QScreen *screen = QGuiApplication::primaryScreen();
    const int screenAngle = screen->angleBetween(screen->nativeOrientation(), screen->orientation());
    int rotation;
    if (cameraInfo.position() == QCamera::BackFace) {
        rotation = (cameraInfo.orientation() - screenAngle) % 360;
    } else {
        // Front position, compensate the mirror
        rotation = (360 - cameraInfo.orientation() + screenAngle) % 360;
    }
    int a = cameraInfo.orientation();
    qDebug() << "Camer angle:" << a << rotation;
    return rotation;
}

int MainWindow::CamerOrientation(const QCamera camera)
{
    QCameraInfo cameraInfo(camera); // needed to get the camera sensor position and orientation
    
    return CamerOrientation(cameraInfo);
}

void MainWindow::on_actionDetector_triggered()
{
    m_Operate = DETECETOR;
    ui->actionDetector->setEnabled(true);
    ui->actionLandmarker->setEnabled(false);
    ui->actionRecognizer->setEnabled(false);
}

void MainWindow::on_actionLandmarker_triggered()
{
    m_Operate = LANDMARKERE;
    ui->actionDetector->setEnabled(false);
    ui->actionLandmarker->setEnabled(true);
    ui->actionRecognizer->setEnabled(false);
}

void MainWindow::on_actionRecognizer_triggered()
{
    m_Operate = RECONGNIZER;
    ui->actionDetector->setEnabled(false);
    ui->actionLandmarker->setEnabled(false);
    ui->actionRecognizer->setEnabled(true);
}

void MainWindow::on_actionSet_model_path_triggered()
{
    m_Display.SetModelPath(QFileDialog::getExistingDirectory(this, tr("Open model file path"), qApp->applicationDirPath()));
}
