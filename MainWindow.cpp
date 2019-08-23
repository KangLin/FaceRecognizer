#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QCameraInfo>
#include <QGuiApplication>
#include <QScreen>

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
        
        m_Display.SetCameraAngle(CamerOrientation(QCameraInfo::availableCameras().at(0)));
        Q_ASSERT(check);
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
