#include "mainwidget.h"
#include <mainwindow.h>
#include "logindlg.h"

MainWidget::MainWidget(QWidget *parent)
    : QStackedWidget{parent}
{
    window = new MainWindow();
    login = new LoginDlg();

    this->insertWidget(0, login);
    this->insertWidget(1, window);
    connect(login, SIGNAL(sigloginSuccess(int)), SLOT(switchStack(int)));
    connect(login, SIGNAL(sigidInfo(QString)), window, SLOT(slotDoctorInfo(QString)));

    resize(1248, 681);
}

void MainWidget::switchStack(int n)
{
    switch(n)
    {
        case 0:
        {
            this->setCurrentIndex(n);
            break;
        }
        case 1:
        {
            this->setCurrentIndex(n);
            break;
        }
    }

}
