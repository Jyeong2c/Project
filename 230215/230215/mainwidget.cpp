#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QStackedWidget{parent}
{
    window = new MainWindow();  // MainWindow 할당
    login = new LoginForm();    // LoginForm 할당

    this->insertWidget(0, login);   // 인덱스 0 : LoginForm
    this->insertWidget(1, window);  // 인덱스 1 : MainWindow

    connect(login, SIGNAL(sig_loginSuccess(int)), SLOT(switchStack(int)));
    connect(login, SIGNAL(sig_idInfo(QString)), window, SLOT(slot_doctorInfo(QString)));
    connect(window, SIGNAL(sig_logout(int)), SLOT(switchStack(int)));
    connect(window, SIGNAL(sig_logClear()), login, SLOT(slot_logClear()));

    this->setMinimumSize(400, 400);
    resize(720, 600);

    this->setStyleSheet("background-color: rgb(223, 231, 245);");
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


