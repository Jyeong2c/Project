#include "mainwindow.h"
//#include "layout.h"
//#include "fmx.h"
//#include "listalbum.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
