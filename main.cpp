#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    MainWindow w;
    
#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    w.show();
#endif
    
    return a.exec();
}
