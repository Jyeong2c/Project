#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QStackedWidget{parent}
{
    window = new MainWindow();
    login = new LoginForm();

    this->insertWidget(0, login);
    this->insertWidget(1, window);
    connect(login, SIGNAL(sig_loginSuccess(int)), SLOT(switchStack(int)));
    connect(login, SIGNAL(sig_idInfo(QString)), window, SLOT(slot_doctorInfo(QString)));
    connect(window, SIGNAL(sig_logout(int)), SLOT(switchStack(int)));
    connect(window, SIGNAL(sig_logClear()), login, SLOT(slot_logClear()));


    this->setMinimumSize(400, 400);
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


