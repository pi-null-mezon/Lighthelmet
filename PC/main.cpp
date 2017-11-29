#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN32
    setlocale(LC_CTYPE,"Rus");
#endif

    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);    
    MainWindow w;
    w.show();

    return a.exec();
}
