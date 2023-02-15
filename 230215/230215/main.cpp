#include "mainwidget.h"

#include <QStackedWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget main;
    main.show();
    return a.exec();
}
