#include "mainwindow.h"
#include "ui_mainwindow.h"

//QGraphicsView 안에 QGraphicsView를 넣는 개념
class QGraphicsView;
class QGraphicsScene;

class QGraphicLineItem;

#include <QActionGroup>
#include "scene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lineAction = new QAction("Draw Line", this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setIcon(QIcon(":/icons/line.png"));

    selectAction = new QAction("Select object", this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setIcon(QIcon(":icons/select.png"));
    selectAction->setCheckable(true);

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(lineAction);
    actionGroup->addAction(selectAction);
}

void MainWindow::actionGroupClicked(QAction* action)
{
    scene->setMode(Scene::Mode(action->data().toInt()));
}


void MainWindow::createActions()
{

}
void MainWindow::createConnections()
{
    connect(actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(actionGroupClicked(QAction*)));
}
void MainWindow::createToolBar()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

