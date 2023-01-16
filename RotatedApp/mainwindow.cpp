#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(250,250);
    createSceneAndView();
    createGraphicsItems();
    createToolBarAndAction();

    QObject::connect(rotateAction, SIGNAL(triggered()),
                     this, SLOT(rotate90()));
}

void MainWindow::createSceneAndView(){
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,230,200);
    view = new QGraphicsView(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
    setCentralWidget(view);
}

void MainWindow::createGraphicsItems(){
    QGraphicsEllipseItem* ellipseItem1 =
            new QGraphicsEllipseItem(QRectF(0,0,80,120));
    ellipseItem1->setPos(60,40);
    ellipseItem1->setBrush(QColor("#1E90FF"));
    ellipseItem1->setZValue(1.0);

    QGraphicsRectItem* rectItem =
            new QGraphicsRectItem(QRectF(0,0,50,80));
    rectItem->setPos(120,60);
    rectItem->setBrush(QColor("#EE82EE"));
    rectItem->setZValue(2.0);

    QGraphicsEllipseItem* ellipseItem2 =
            new QGraphicsEllipseItem(QRectF(0,0,70,70));
    ellipseItem2->setPos(80,90);
    ellipseItem2->setBrush(QColor("#C0C0C0"));
    ellipseItem2->setZValue(3.0);

    scene->addItem(ellipseItem1);
    scene->addItem(rectItem);
    scene->addItem(ellipseItem2);

    foreach(QGraphicsItem* item, scene->items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable);
        item->setFlag(QGraphicsItem::ItemIsMovable);
    }
}

void MainWindow::createToolBarAndAction(){
    editToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, editToolBar);

    rotateAction = new QAction("Rotate 90", this);
    rotateAction->setIcon(QIcon("./rotate90.png"));

    editToolBar->addAction(rotateAction);
}

void MainWindow::rotate90(){
    int x = 45;
    QGraphicsItemGroup* gr = scene->createItemGroup(
                scene->selectedItems());
    QPointF offset = gr->sceneBoundingRect().center();

    QTransform transform;
    transform.translate(offset.x(),offset.y());
    transform.rotate(45);
    transform.translate(-offset.x(), -offset.y());
    gr->setTransform(transform);

    QMessageBox::information(this, "rotate",
                             QString("rotated : %1").arg(x));

    scene->destroyItemGroup(gr);
    scene->update();
}
