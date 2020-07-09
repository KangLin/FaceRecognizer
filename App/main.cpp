/**
  @author: Kang Lin<kl222@126.com>
  */

#include <QApplication>
#if defined(Q_OS_ANDROID)
    #include <QtAndroidExtras/QtAndroid>
#endif

#include "MainWindow.h"

#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "RabbitCommonDir.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif

#include "Log.h"

int main(int argc, char *argv[])
{
    LOG_INITIALIZER;

#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif
    QApplication app(argc, argv);   
    app.setApplicationName("FaceRecognizer");

#ifdef RABBITCOMMON
    LOG_MODEL_INFO("main", "GetDirApplication:%s",
                    RabbitCommon::CDir::Instance()->GetDirApplication().toStdString().c_str());
    
    RabbitCommon::CTools::Instance()->Init();

    QString szTranslator = RabbitCommon::CDir::Instance()->GetDirTranslations()
            + "/" + qApp->applicationName()
            + "App_" + QLocale::system().name() + ".qm";
    qDebug() << "Translator:" << szTranslator;
    QTranslator translator;
    translator.load(szTranslator);
    app.installTranslator(&translator);
#endif
   
    app.setApplicationDisplayName(QObject::tr("Face recognizer"));
    
#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/image/FaceRecognizer"));
    if(!pUpdate->GenerateUpdateXml()) 
        return 0; 
#endif

    QFile f(":/qss/black.qss");
    if(f.open(QFile::ReadOnly))
    {
        QString szStyle = f.readAll();
        qDebug() << "Style:" << szStyle;
        qApp->setStyleSheet(szStyle);
        f.close();
    }

    app.addLibraryPath(RabbitCommon::CDir::Instance()->GetDirPlugins());

    MainWindow w;
    
#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    w.show();
#endif
    
    int nRet = app.exec();
     
    LOG_CLEAN;
 
    return nRet;
}
