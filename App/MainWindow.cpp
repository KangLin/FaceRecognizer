#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCameraInfo>
#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#ifdef RABBITCOMMON
    #include "FrmUpdater/FrmUpdater.h"
    #include "DlgAbout/DlgAbout.h"
    #include "RabbitCommonDir.h"
#endif

#include "FrmRecognizer.h"
#include "FrmRegister.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCamera(nullptr)
{
    ui->setupUi(this);

    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    m_szModelFile = set.value("ModuleDir").toString();
    qDebug() << "Model files:" << m_szModelFile;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << "Camer name:" << cameraInfo.deviceName();
    }

    if(QCameraInfo::availableCameras().size() > 0)
    {
        m_pCamera = new QCamera(QCameraInfo::availableCameras().at(0));
        QCameraViewfinderSettings viewfinderSettings = m_pCamera->viewfinderSettings();

        m_pCamera->load();

        qInfo() << "Camera support:";
        qInfo() << "Resolutions:" << m_pCamera->supportedViewfinderResolutions(m_pCamera->viewfinderSettings());
        QList<QCamera::FrameRateRange> ranges =  m_pCamera->supportedViewfinderFrameRateRanges();
        for(auto &rang: ranges)
        {
            qInfo() << "Frame rate range:" << rang.maximumFrameRate << rang.maximumFrameRate;
        }
        qInfo() << "Pixel formate:" << m_pCamera->supportedViewfinderPixelFormats(m_pCamera->viewfinderSettings());

//        viewfinderSettings.setResolution(640, 480);
//        viewfinderSettings.setMinimumFrameRate(10.0);
//        viewfinderSettings.setMaximumFrameRate(30.0);
//        m_pCamera->setViewfinderSettings(viewfinderSettings);
        m_pCamera->unload();

        qInfo() << "Current:";
        qInfo() << "Resolutions:" << viewfinderSettings.resolution();
        qInfo() << "Frame rate:" << viewfinderSettings.minimumFrameRate() << viewfinderSettings.maximumFrameRate();
        qInfo() << "Pixel formate:" << viewfinderSettings.pixelFormat();
        qInfo() << "" << viewfinderSettings.pixelAspectRatio();

        //*/
        m_pCamera->setViewfinder(&m_CaptureFrame);
    } else {
        QMessageBox::warning(nullptr, tr("Warning"), tr("The devices is not camera"));
    }

    on_actionRecognizer_triggered();
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

void MainWindow::on_actionSet_model_path_triggered()
{
   m_szModelFile = QFileDialog::getExistingDirectory(this,
                        tr("Open model file path"), qApp->applicationDirPath());
   QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                 QSettings::IniFormat);
   set.setValue("ModuleDir", m_szModelFile);
}

void MainWindow::on_actionRecognizer_triggered()
{
    CFrmRecognizer *pRecognizer = new CFrmRecognizer(this);
    if(!pRecognizer)
        return;
    
    if(!m_szModelFile.isEmpty())
        pRecognizer->SetModelPath(m_szModelFile);
    this->setCentralWidget(pRecognizer);
    bool check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QVideoFrame &)),
                         pRecognizer, SLOT(slotDisplay(const QVideoFrame &)));
    Q_ASSERT(check);
#if defined (Q_OS_ANDROID)
    pRecognizer->SetCameraAngle(CamerOrientation(QCameraInfo::availableCameras().at(0)));
#else           
    pRecognizer->SetCameraAngle(180);
#endif
}

void MainWindow::on_actionRegister_triggered()
{
    CFrmRegister *pRegister = new CFrmRegister(this);
    if(!pRegister)
        return;
    
    if(!m_szModelFile.isEmpty())
        pRegister->SetModelPath(m_szModelFile);
    this->setCentralWidget(pRegister);
    bool check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QVideoFrame &)),
                         pRegister, SLOT(slotDisplay(const QVideoFrame &)));
    Q_ASSERT(check);
#if defined (Q_OS_ANDROID)
    pRegister->SetCameraAngle(CamerOrientation(QCameraInfo::availableCameras().at(0)));
#else           
    pRegister->SetCameraAngle(180);
#endif
}

void MainWindow::on_actionAbout_A_triggered()
{
#ifdef RABBITCOMMON
    CDlgAbout about(this);
    //about.m_AppIcon = QPixmap(":/icon/App");
    about.m_szHomePage = "https://github.com/KangLin/FaceRecognizer";
    if(about.isHidden())
#if defined (Q_OS_ANDROID)
        about.showMaximized();
#endif
        about.exec();
#endif
}

void MainWindow::on_actionUpdate_U_triggered()
{
#ifdef RABBITCOMMON
    CFrmUpdater* m_pfrmUpdater = new CFrmUpdater();
    //m_pfrmUpdater->SetTitle(QPixmap(":/icon/App"));
    #if defined (Q_OS_ANDROID)
        m_pfrmUpdater->showMaximized();
    #else
        m_pfrmUpdater->show();
    #endif
#endif
}
