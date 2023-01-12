#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->BrushtoolButton->setIcon(QIcon("draw2.png"));
    ui->BrushtoolButton->setIconSize(QSize(30,100));
    ui->BrushtoolButton->setText("Brush");
    ui->BrushtoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    ui->ColortoolButton->setIcon(QIcon("color.png"));
    ui->ColortoolButton->setIconSize(QSize(30,100));
    ui->ColortoolButton->setText("Color");
    ui->ColortoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


}

MainWindow::~MainWindow()
{
    delete ui;
}

