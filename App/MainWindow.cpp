#include "MainWindow.h"
#include "ui_MainWindow.h"

#ifdef RABBITCOMMON
    #include "FrmUpdater/FrmUpdater.h"
    #include "DlgAbout/DlgAbout.h"
    #include "RabbitCommonDir.h"
#endif

#include "FrmDisplay.h"
#include "ParameterFactory.h"
#include "FrmRegisterImage.h"
#include "FrmRecognizerImage.h"
#include "ManageRegisterVideo.h"
#include "ManageRecognizerVideo.h"
#include "Factory.h"
#include "Log.h"

#include <QIcon>
#include <QCameraInfo>
#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QComboBox>
#include <QActionGroup>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCamera(nullptr)
{
    ui->setupUi(this);

    CLog::Instance()->SaveFile(QStandardPaths::writableLocation(
                                   QStandardPaths::TempLocation)
                               + QDir::separator()
                               + qApp->applicationName() + ".log");
    //Init menu
    ui->actionStart->setIcon(QIcon(":/image/Start"));
    ui->actionStart->setText(tr("Start"));
    ui->actionStart->setToolTip(tr("Start"));
    ui->actionStart->setStatusTip(tr("Start"));
    
    QActionGroup *pViewGroup = new QActionGroup(this);
    pViewGroup->addAction(ui->actionDisplay);
    pViewGroup->addAction(ui->actionRegisterImage);
    pViewGroup->addAction(ui->actionRegisterImage_directory);
    pViewGroup->addAction(ui->actionRegisterVideo);
    pViewGroup->addAction(ui->actionRecognizerImage);
    pViewGroup->addAction(ui->actionRecognizerVideo);

    QActionGroup *pViewGroup1 = new QActionGroup(this);
    pViewGroup1->addAction(ui->actionFile);
    pViewGroup1->addAction(ui->actionCamera);
    
    QActionGroup *pCameraAngle = new QActionGroup(this);
    pCameraAngle->addAction(ui->action0);
    pCameraAngle->addAction(ui->action90);
    pCameraAngle->addAction(ui->action180);
    pCameraAngle->addAction(ui->action270);
    bool check = connect(pCameraAngle, SIGNAL(triggered(QAction *)),
                         this, SLOT(slotCameraOrientation(QAction *)));
    Q_ASSERT(check);
    
    if(!QCameraInfo::availableCameras().isEmpty())
    {
        QComboBox *cmbCameras = new QComboBox(ui->toolBar);
        if(cmbCameras)
        {
            ui->actionCamera->setChecked(true);
            cmbCameras->setToolTip(tr("Select camera"));
            cmbCameras->setStatusTip(tr("Select camera"));
            ui->toolBar->addWidget(cmbCameras);
            connect(cmbCameras, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(slotCameraChanged(int)));
            QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
            foreach (const QCameraInfo &cameraInfo, cameras) {
                //qDebug() << "Camer name:" << cameraInfo.deviceName();
                cmbCameras->addItem(cameraInfo.description());
            }
        }
    } else 
        ui->actionFile->setChecked(true);

#ifdef RABBITCOMMON  
    CFrmUpdater updater;
    ui->actionUpdate_U->setIcon(updater.windowIcon());
#endif

#ifdef HAVE_SEETA_FACE
    if(CFactory::Instance()->GetDatabase()->GetTableRegister()->IsExistNo())
    {
        ui->actionRecognizerVideo->setChecked(true);
        on_actionRecognizerVideo_triggered();
    } else {
        ui->actionRegisterVideo->setChecked(true);
        on_actionRegisterVideo_triggered();
    }
#else
    ui->menuRegister->setEnabled(false);
    ui->menuRecognizer->setEnabled(false);
    
    ui->actionDisplay->setChecked(true);
    on_actionDisplay_triggered();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotCameraChanged(int index)
{
    if(QCameraInfo::availableCameras().size() > 0 && index >= 0)
    {
        if(m_pCamera)
        {
            m_pCamera->unload();
            delete m_pCamera;
        }

        m_pCamera = new QCamera(QCameraInfo::availableCameras().at(index));

        /*        
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
        
        m_CaptureFrame.SetCameraAngle(CamerOrientation(index));
        m_pCamera->setViewfinder(&m_CaptureFrame);
    } else {
        QMessageBox::warning(nullptr, tr("Warning"), tr("The devices is not camera"));
    }
}

void MainWindow::slotCameraOrientation(QAction *pAction)
{
    int nAngle = 0;
    if(pAction == ui->action0)
        nAngle = 0;
    if(pAction == ui->action90)
        nAngle = 90;
    if(pAction == ui->action180)
        nAngle = 180;
    if(pAction == ui->action270)
        nAngle = 270;
    m_CaptureFrame.SetCameraAngle(nAngle);
}

void MainWindow::on_actionFile_triggered()
{
#ifdef RABBITCOMMON
    QString szFile = RabbitCommon::CDir::GetOpenFileName(this,
                                   tr("Open file"),
                                   qApp->applicationDirPath());
    if(szFile.isEmpty())
        return;
    QUrl url = QUrl::fromLocalFile(szFile);
    m_Player.setMedia(url);
    m_Player.setVideoOutput(&m_CaptureFrame);
#endif
}

void MainWindow::on_actionStart_triggered()
{
    if(ui->actionStart->isChecked())
    {
        if(m_pCamera && ui->actionCamera->isChecked())
        {
            if(m_pCamera->isAvailable())
                m_pCamera->stop();
            m_pCamera->start();
        } else {
            m_Player.stop();
            m_Player.play();
        }
            
        ui->actionStart->setText(tr("Stop"));
        ui->actionStart->setToolTip(tr("Stop"));
        ui->actionStart->setStatusTip(tr("Stop"));
        ui->actionStart->setIcon(QIcon(":/image/Stop"));
    } else {
        if(m_pCamera && ui->actionCamera->isChecked())
            m_pCamera->stop();
        else
            m_Player.stop();
        
        ui->actionStart->setIcon(QIcon(":/image/Start"));
        ui->actionStart->setText(tr("Start"));
        ui->actionStart->setToolTip(tr("Start"));
        ui->actionStart->setStatusTip(tr("Start"));
    }
}

int MainWindow::CamerOrientation(int index)
{
    if(index < 0 || index >= QCameraInfo::availableCameras().length())
        return -1;
    
    QCameraInfo cameraInfo = QCameraInfo::availableCameras().at(index);

    // Get the current display orientation
    const QScreen *screen = QGuiApplication::primaryScreen();
    const int screenAngle = screen->angleBetween(screen->nativeOrientation(),
                                                 screen->orientation());
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

void MainWindow::on_actionSet_model_path_triggered()
{
#ifdef RABBITCOMMON
   QString szFile = RabbitCommon::CDir::GetOpenDirectory(this,
                        tr("Open model file path"),
                        CParameterFactory::Instance()->GetParameterDetector()->GetModelPath());
   QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                 QSettings::IniFormat);
   set.setValue("ModuleDir", szFile);
   CParameterFactory::Instance()->SetModelPath(szFile);
#endif
}

void MainWindow::on_actionRegisterImage_triggered()
{
    m_CaptureFrame.disconnect();
    CFrmRegisterImage* pImage = new CFrmRegisterImage(this);
    setCentralWidget(pImage);
}

void MainWindow::on_actionRegisterImage_directory_triggered()
{
    
}

void MainWindow::on_actionRegisterVideo_triggered()
{
    CManageRegisterVideo *pManage = new CManageRegisterVideo(this);
    m_CaptureFrame.disconnect();
    bool check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QImage &)),
                          pManage, SIGNAL(sigCaptureFrame(const QImage &)));
    Q_ASSERT(check);
    setCentralWidget(pManage);
}

void MainWindow::on_actionRecognizerImage_triggered()
{
    CFrmRecognizerImage* pImage = new CFrmRecognizerImage(this);
    m_CaptureFrame.disconnect();
    setCentralWidget(pImage);
}

void MainWindow::on_actionRecognizerVideo_triggered()
{
    CManageRecognizerVideo *pManage = new CManageRecognizerVideo();
    m_CaptureFrame.disconnect();
    bool check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QImage&)),
                         pManage, SIGNAL(sigCaptureFrame(const QImage&)));
    Q_ASSERT(check);
    setCentralWidget(pManage);
}

void MainWindow::on_actionDisplay_triggered()
{
    CFrmDisplay *pDisplay = new CFrmDisplay(this);
    if(!pDisplay)
        return;
    m_CaptureFrame.disconnect();
    bool check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QImage &)),
                          pDisplay, SLOT(slotDisplay(const QImage &)));
    Q_ASSERT(check);
    this->setCentralWidget(pDisplay);
}

void MainWindow::on_actionAbout_A_triggered()
{
#ifdef RABBITCOMMON
    CDlgAbout about(this);
    about.m_AppIcon = QImage(":/image/FaceRecognizer");
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
    m_pfrmUpdater->SetTitle(QImage(":/image/FaceRecognizer"));
    #if defined (Q_OS_ANDROID)
        m_pfrmUpdater->showMaximized();
    #else
        m_pfrmUpdater->show();
    #endif
#endif
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionOpen_log_file_triggered()
{
    CLog::Instance()->OpneFile();
}
