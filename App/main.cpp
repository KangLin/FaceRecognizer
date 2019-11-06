#include <QApplication>

#include "MainWindow.h"

#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "RabbitCommonDir.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    a.setApplicationName("FaceRecognizer");

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();

    QTranslator translator;
    translator.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
                    + "/" + qApp->applicationName()
                    + "App_" + QLocale::system().name() + ".qm");
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
