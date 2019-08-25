#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("FaceRecognizer");
    a.setApplicationDisplayName("FaceRecognizer");
    
    MainWindow w;
    
#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    w.show();
#endif
    
    return a.exec();
}
