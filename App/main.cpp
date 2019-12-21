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

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif
    QApplication a(argc, argv);
    a.setApplicationName("FaceRecognizer");

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();

    QString szTranslator = RabbitCommon::CDir::Instance()->GetDirTranslations()
            + "/" + qApp->applicationName()
            + "App_" + QLocale::system().name() + ".qm";
    qDebug() << "Translator:" << szTranslator;
    QTranslator translator;
    translator.load(szTranslator);
    a.installTranslator(&translator);
#endif
   
    a.setApplicationDisplayName(QObject::tr("Face recognizer"));
    
#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/image/FaceRecognizer"));
    if(!pUpdate->GenerateUpdateXml()) 
        return 0; 
#endif

    MainWindow w;
    
#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    w.show();
#endif
    
    return a.exec();
}
