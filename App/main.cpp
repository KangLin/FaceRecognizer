/**
  @author: Kang Lin<kl222@126.com>
  */


#ifdef RABBITCOMMON
#include "RabbitCommonTools.h"
#include "RabbitCommonDir.h"
#include "FrmUpdater/FrmUpdater.h"
#endif

#include "MainWindow.h"

#include <QApplication>

#include <QLoggingCategory>
static Q_LOGGING_CATEGORY(log, "App.main")

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion(FaceRecognizer_VERSION);
    app.setApplicationName("FaceRecognizer");

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();

    QString szTranslator = RabbitCommon::CDir::Instance()->GetDirTranslations()
            + "/" + QCoreApplication::applicationName()
            + "App_" + QLocale::system().name() + ".qm";
    qDebug(log) << "Translator:" << szTranslator;
    QTranslator translator;
    bool bRet = translator.load(szTranslator);
    if(bRet){
        bRet = app.installTranslator(&translator);
        if(!bRet)
            qCritical(log) << "Install translator fail." << szTranslator;
    }
    else
        qCritical(log) << "Load translator fail." << szTranslator;
#endif

    app.setApplicationDisplayName(QObject::tr("Face recognizer"));

#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/image/FaceRecognizer"));
    if(app.arguments().length() > 1) {
        pUpdate->GenerateUpdateJson();
        pUpdate->GenerateUpdateXml();
        return 0;
    }
#endif

    QFile f(":/qss/black.qss");
    if(f.open(QFile::ReadOnly))
    {
        QString szStyle = f.readAll();
        qDebug(log) << "Style:" << szStyle;
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

    return nRet;
}
