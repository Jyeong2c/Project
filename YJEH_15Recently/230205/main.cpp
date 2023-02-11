//#include "mainwindow.h"
//#include "layout.h"
//#include "fmx.h"
//#include "loginform.h"
//#include "keyfeaturesform.h"
//#include "teethform.h"
#include "mainwidget.h"

#include <QStackedWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget main;
    main.show();
    return a.exec();


//    QApplication a(argc, argv);
//    Layout L;
//    L.show();
//    return a.exec();

//    QApplication a(argc, argv);
//    Layout L;
//    L.setStyleSheet("background:rgb(32, 56, 100)");
//    L.show();
//    return a.exec();


//    QApplication a(argc, argv);
//    FMX X;
//    X.show();
//    return a.exec();

//    QApplication a(argc, argv);
//    Listalbum A;
//    A.show();
//    return a.exec();

//    QApplication a(argc, argv);
//    LoginForm n;
//    n.show();
//    return a.exec();

//    QApplication a(argc, argv);
//    KeyFeaturesForm k;
//    k.show();
//    return a.exec();

//    QApplication a(argc, argv);
//    TeethForm t;
//    t.show();
//    return a.exec();
}
