#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    Download d1;

//    d1.setTarget(QString("http://192.168.0.12:40000/uploads/"));
//    //d1.setTarget(QString("http://192.168.0.12:40000/uploads/1675496062085.png"));

//    d1.download();
//    Download::connect(&d1, SIGNAL(done()), &a, SLOT(quit()));

    return a.exec();
}
