/**
  @author: Kang Lin<kl222@126.com>
  */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#ifdef RABBITCOMMON
    #include "FrmUpdater/FrmUpdater.h"
    #include "DlgAbout/DlgAbout.h"
    #include "RabbitCommonDir.h"
#endif

#include "FrmDisplay.h"
#include "FrmRegisterImage.h"
#include "FrmRecognizerImage.h"
#include "ManageRegisterVideo.h"
#include "ManageRecognizerVideo.h"
#include "FactoryFace.h"
#include "Log.h"
#include "DlgLog.h"

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
#include <QDockWidget>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCamera(nullptr)
{
    LoadStyle();
    ui->setupUi(this);
    CLog::Instance()->SetSaveFile(QStandardPaths::writableLocation(
                                   QStandardPaths::TempLocation)
                               + QDir::separator()
                               + qApp->applicationName() + ".log");
    //Init menu
    ui->actionStart->setIcon(QIcon(":/image/Start"));
    ui->actionStart->setText(tr("Start"));
    ui->actionStart->setToolTip(tr("Start"));
    ui->actionStart->setStatusTip(tr("Start"));
    
    // Operator menu
    QActionGroup *pViewGroup = new QActionGroup(this);
    pViewGroup->addAction(ui->actionDisplay);
    pViewGroup->addAction(ui->actionRegisterImage);
    pViewGroup->addAction(ui->actionRegisterImage_directory);
    pViewGroup->addAction(ui->actionRegisterVideo);
    pViewGroup->addAction(ui->actionRecognizerImage);
    pViewGroup->addAction(ui->actionRecognizerVideo);

    // Source menu
    QActionGroup *pSourceGroup = new QActionGroup(this);
    pSourceGroup->addAction(ui->actionFile);
    pSourceGroup->addAction(ui->actionCamera);
    
    // Camera
    if(!QCameraInfo::availableCameras().isEmpty())
    {
        QComboBox *cmbCameras = new QComboBox(ui->toolBar);
        if(cmbCameras)
        {
            ui->actionCamera->setChecked(true);
            cmbCameras->setToolTip(tr("Select camera"));
            cmbCameras->setStatusTip(tr("Select camera"));
            ui->toolBar->addWidget(cmbCameras);
            QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
            foreach (const QCameraInfo &cameraInfo, cameras) {
                //                LOG_MODEL_DEBUG("MainWindows", "Camer name: %s",
                //                                cameraInfo.deviceName().toStdString().c_str());
                cmbCameras->addItem(cameraInfo.description());
            }

            connect(cmbCameras, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(slotCameraChanged(int)));

#ifdef RABBITCOMMON
            QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                          QSettings::IniFormat);
            int index = set.value("CameraIndex", 0).toInt();
            cmbCameras->setCurrentIndex(index);
            slotCameraChanged(index);
#endif
            
            InitCamerOrientation();
        }
        
    } else 
        ui->actionFile->setChecked(true);

#ifdef RABBITCOMMON  
    CFrmUpdater updater;
    ui->actionUpdate_U->setIcon(updater.windowIcon());
#endif

    createDockPerameters();

    // Load model
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szPath = set.value("ModuleDir",
                               RabbitCommon::CDir::Instance()->GetDirData(false)
                               + QDir::separator() + "model").toString();
    setModelPath(szPath);

    if(CFactoryFace::Instance()->bIsValid())
    {
        if(CFactoryFace::Instance()->GetDatabase()->GetTableRegister()->IsExistNo())
        {
            ui->actionRecognizerVideo->setChecked(true);
            on_actionRecognizerVideo_triggered();
        } else {
            ui->actionRegisterVideo->setChecked(true);
            on_actionRegisterVideo_triggered();
        }
    } else {
        ui->menuRegister->setEnabled(false);
        ui->menuRecognizer->setEnabled(false);

        ui->actionDisplay->setChecked(true);
        on_actionDisplay_triggered();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::LoadStyle()
{
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szFile = set.value("Sink",
                  RabbitCommon::CDir::Instance()->GetDirApplicationInstallRoot()
                  + QDir::separator()
                  + "data" + QDir::separator()
                  + "style" + QDir::separator()
                  + "black.qss").toString();
    return  LoadStyle(szFile);
}

int MainWindow::LoadStyle(const QString &szFile)
{
    if(szFile.isEmpty())
        qApp->setStyleSheet("");
    else
    {
        QFile file(szFile);
        if(file.open(QFile::ReadOnly))
        {
            QString stylesheet= file.readAll();
            qApp->setStyleSheet(stylesheet);
            file.close();
        }
        else
        {
            qDebug() << "file open file fail:" << szFile;                       
        }
    }
    return 0;
}

void MainWindow::on_actionStyle_triggered()
{
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szFile = set.value("Sink",
                  RabbitCommon::CDir::Instance()->GetDirApplicationInstallRoot()
                  + QDir::separator()
                  + "data" + QDir::separator()
                  + "style" + QDir::separator()
                  + "black.qss").toString();
    szFile = RabbitCommon::CDir::GetOpenFileName(this, tr("Open sink"),
                 szFile,
                 tr("Style files(*.qss)"));
    if(szFile.isEmpty()) return;
    LoadStyle(szFile);
    set.setValue("Sink", szFile);
}

void MainWindow::slotCameraChanged(int index)
{
    if(!(QCameraInfo::availableCameras().size() > 0 && index >= 0))
    {
        QMessageBox::warning(nullptr, tr("Warning"), tr("The devices is not camera"));
        return;
    }
    
    if(m_pCamera)
    {
        m_pCamera->unload();
        delete m_pCamera;
    }
    
    m_pCamera = new QCamera(QCameraInfo::availableCameras().at(index));
    if(nullptr == m_pCamera) return;
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
    
//    viewfinderSettings.setResolution(640, 480);
//    viewfinderSettings.setMinimumFrameRate(10.0);
//    viewfinderSettings.setMaximumFrameRate(30.0);
//    m_pCamera->setViewfinderSettings(viewfinderSettings);
    m_pCamera->unload();
    
    qInfo() << "Current:";
    qInfo() << "Resolutions:" << viewfinderSettings.resolution();
    qInfo() << "Frame rate:" << viewfinderSettings.minimumFrameRate() << viewfinderSettings.maximumFrameRate();
    qInfo() << "Pixel formate:" << viewfinderSettings.pixelFormat();
    qInfo() << "" << viewfinderSettings.pixelAspectRatio();
    
    //*/
    
    m_CaptureFrame.SetCameraAngle(CamerOrientation(index));
    m_pCamera->setViewfinder(&m_CaptureFrame);
    
    QCameraFocus* focus = m_pCamera->focus();
    if(focus)
    {
        QCameraFocus::FocusMode focusMode = QCameraFocus::AutoFocus;
//        if(focus->isFocusModeSupported(QCameraFocus::AutoFocus))
//        {
//            focusMode = QCameraFocus::AutoFocus;
//        } else if(focus->isFocusModeSupported(QCameraFocus::QCameraFocus::ContinuousFocus))
//        {
//            focusMode = QCameraFocus::ContinuousFocus;
//            */            } else if(focus->isFocusModeSupported(QCameraFocus::MacroFocus))
//            focusMode = QCameraFocus::MacroFocus;
//        else
//            focusMode = QCameraFocus::ManualFocus;
        LOG_MODEL_DEBUG("MainWindow", "focusMode:0x%x", focusMode);
        focus->setFocusMode(focusMode);
        
        focus->setFocusPointMode(QCameraFocus::FocusPointAuto);
        QList<QCameraFocusZone> zones = focus->focusZones();
//        foreach (QCameraFocusZone zone, zones) {
//            if (zone.status() == QCameraFocusZone::Focused) {
//                // Draw a green box at zone.area()
//            } else if (zone.status() == QCameraFocusZone::Selected) {
//                // This area is selected for autofocusing, but is not in focus
//                // Draw a yellow box at zone.area()
//            }
//        }
        
    }
    
#ifdef RABBITCOMMON
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    set.setValue("CameraIndex", index);
#endif
    
}

int MainWindow::InitCamerOrientation()
{
    QActionGroup *pCameraAngle = new QActionGroup(this);
    pCameraAngle->addAction(ui->action0);
    pCameraAngle->addAction(ui->action90);
    pCameraAngle->addAction(ui->action180);
    pCameraAngle->addAction(ui->action270);

    int nAngle = 0;
#ifdef RABBITCOMMON
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    nAngle = set.value("CameraOrientation", nAngle).toInt();
#endif
    m_CaptureFrame.SetCameraAngle(nAngle);
    switch (nAngle) {
    case 0:
        ui->action0->setChecked(true);
        break;
    case 90:
        ui->action90->setChecked(true);
        break;
    case 180:
        ui->action180->setChecked(true);
        break;
    case 270:
        ui->action270->setChecked(true);
        break;
    }
    
    bool check = connect(pCameraAngle, SIGNAL(triggered(QAction *)),
                         this, SLOT(slotCameraOrientation(QAction *)));
    Q_ASSERT(check);
    return 0;    
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

#ifdef RABBITCOMMON
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    set.setValue("CameraOrientation", nAngle);
#endif
}

void MainWindow::on_actionFile_triggered()
{
#ifdef RABBITCOMMON
    QString szFile;
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    szFile = set.value("SourceFile").toString();

    szFile = RabbitCommon::CDir::GetOpenFileName(this,
                                   tr("Open file"),
                                   szFile);
    if(szFile.isEmpty())
        return;

    set.setValue("SourceFile", szFile);
    

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
            
            QString szFile;
            QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                          QSettings::IniFormat);
            szFile = set.value("SourceFile").toString();
        
            QUrl url = QUrl::fromLocalFile(szFile);
            m_Player.setMedia(url);
            m_Player.setVideoOutput(&m_CaptureFrame);
            
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
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->setOrientationUpdateMask(Qt::LandscapeOrientation
                                     | Qt::PortraitOrientation
                                     | Qt::InvertedLandscapeOrientation
                                     | Qt::InvertedPortraitOrientation);
    bool check = connect(screen, SIGNAL(orientationChanged(Qt::ScreenOrientation)),
            this, SLOT(slotScreenOrientationChanged(Qt::ScreenOrientation)));
    Q_ASSERT(check);

    int screenAngle = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
    qDebug() << "orientation:" << screen->orientation()
             << screen->nativeOrientation()
             << screen->orientationUpdateMask();
    screenAngle = screen->angleBetween(screen->nativeOrientation(),
                                                 screen->orientation());
#endif
    qDebug() << "screenAngle:" << screenAngle
             << "camer orientation:" << cameraInfo.orientation();
    int rotation;
    if (cameraInfo.position() == QCamera::BackFace) {
        rotation = (cameraInfo.orientation() - screenAngle) % 360;
    } else {
        // Front position, compensate the mirror
        rotation = (360 - cameraInfo.orientation() + screenAngle) % 360;
    }
    int a = cameraInfo.orientation();
    LOG_MODEL_DEBUG("MainWindow", "Camer angle: %d; %d", a, rotation);
    qDebug() << "orientation1:" << a << rotation;
    return rotation;
}

void MainWindow::slotScreenOrientationChanged(Qt::ScreenOrientation orientation)
{
    QScreen *screen = QGuiApplication::primaryScreen();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
    qDebug() << "slotScreenOrientationChanged:" << orientation
             <<screen->nativeOrientation()
            << screen->orientation();
#endif
}

void MainWindow::on_actionSet_model_path_triggered()
{
#ifdef RABBITCOMMON
   QString szPath = RabbitCommon::CDir::GetOpenDirectory(this,
                        tr("Open model file path"));
   QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                 QSettings::IniFormat);
   set.setValue("ModuleDir", szPath);
   setModelPath(szPath);
#endif
}

int MainWindow::setModelPath(const QString &szPath)
{
    CFactoryFace::Instance()->setModelPath(szPath);
    if(m_Paramter)
        m_Paramter->slotUpdateParamter();
    return 0;
}

void MainWindow::on_actionRegisterImage_triggered()
{
    m_CaptureFrame.disconnect();
    CFrmRegisterImage* pImage = new CFrmRegisterImage(this);
    setCentralWidget(pImage);
}

void MainWindow::on_actionRegisterImage_directory_triggered()
{}

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
    pDisplay->slotSetAspectRatio(ui->actionKeep_display_aspect_ratio->isChecked());
    bool check = connect(ui->actionKeep_display_aspect_ratio, SIGNAL(triggered(bool)),
                    pDisplay, SLOT(slotSetAspectRatio(bool)));
    Q_ASSERT(check);
    
    m_CaptureFrame.disconnect();
    check = connect(&m_CaptureFrame, SIGNAL(sigCaptureFrame(const QImage &)),
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
    int nRet = CLog::Instance()->OpneFile();
    if(0 == nRet) return;
    
    CDlgLog log(CLog::Instance()->GetSaveFile());
#if defined(Q_OS_ANDROID)
    log.showMaximized();
#endif
    log.exec();
}

void MainWindow::on_actionAiLibraries_triggered(QAction* a)
{
    CFactoryFace::Instance()->SetLibType(a->data().toString(), false);
#ifdef RABBITCOMMON
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    set.setValue("AI_Libraries", a->data().toString());
#endif
}

int MainWindow::createDockPerameters()
{
#if defined (Q_OS_ANDROID)
    bool bHide = true;
#else
    bool bHide = false;
#endif
#ifdef RABBITCOMMON
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    bHide = set.value("ParametersDockHide", bHide).toBool();
#endif

    QDockWidget *dock = new QDockWidget(tr("Set parameters"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    m_Paramter = QSharedPointer<CFrmPara>(new CFrmPara(dock));
    if(!m_Paramter) 
    {
        LOG_MODEL_ERROR("MainWindow", "new CFrmPara fail");
        return -1;
    }
    dock->setWidget(m_Paramter.data());
    if(bHide) dock->hide();
#if defined (Q_OS_ANDROID)
    addDockWidget(Qt::BottomDockWidgetArea, dock);
#else
    addDockWidget(Qt::RightDockWidgetArea, dock);
#endif
    ui->menuTools->addAction(dock->toggleViewAction());
    bool check = connect(dock->toggleViewAction(), SIGNAL(triggered(bool)),
                         this, SLOT(slotParaDock_triggered(bool)));
    Q_ASSERT(check);
//    check = connect(dock, SIGNAL(visibilityChanged(bool)),
//                    this, SLOT(slotParaDock_triggered(bool)));
//    Q_ASSERT(check);

    QActionGroup *pAiGroup = new QActionGroup(this);
    ui->menuAI_libraries->setEnabled(false);
    QVector<CFace*> faces = CFactoryFace::Instance()->GetLibType();
    QString szSelect;
    if(!faces.empty())
        szSelect = faces[0]->GetName();
#ifdef RABBITCOMMON
    QSettings setAILibs(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    szSelect = setAILibs.value("AI_Libraries", szSelect).toString();
#endif
    CFactoryFace::Instance()->SetLibType(szSelect, false);
    foreach(auto f, faces)
    {
        ui->menuAI_libraries->setEnabled(true);
        QAction* pA = ui->menuAI_libraries->addAction(f->GetDescript());
        pA->setStatusTip(f->GetDescript());
        pA->setToolTip(f->GetDescript());
        pAiGroup->addAction(pA);
        pA->setCheckable(true);
        pA->setData(f->GetName());
        if(szSelect == f->GetName())
            pA->setChecked(true);
    }
    check = connect(pAiGroup, SIGNAL(triggered(QAction*)),
                         this, SLOT(on_actionAiLibraries_triggered(QAction*)));
    Q_ASSERT(check);
    check = connect(pAiGroup, SIGNAL(triggered(QAction*)),
                    m_Paramter.data(), SLOT(slotUpdateParamter(QAction*)));
    Q_ASSERT(check);
    m_Paramter->slotUpdateParamter();
    
    return 0;
}

void MainWindow::slotParaDock_triggered(bool checked)
{
#ifdef RABBITCOMMON
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    set.setValue("ParametersDockHide", !checked);
#endif
}
