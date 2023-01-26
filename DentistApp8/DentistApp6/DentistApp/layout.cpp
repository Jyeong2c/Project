#include "layout.h"

#include <QGridLayout>
#include <QGraphicsView>

#include <QPixmap>

Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();

    scene1 = new QGraphicsScene;
    scene2 = new QGraphicsScene;
    scene3 = new QGraphicsScene;
    scene4 = new QGraphicsScene;

    scene1->setBackgroundBrush(Qt::black);
    scene2->setBackgroundBrush(Qt::black);
    scene3->setBackgroundBrush(Qt::black);
    scene4->setBackgroundBrush(Qt::black);

    grid1->setScene(scene1);
    grid2->setScene(scene2);
    grid3->setScene(scene3);
    grid4->setScene(scene4);
}

/* 2 X 2 Grid */
void Layout::gridLayout()
{
    grid1 = new QGraphicsView;
    grid2 = new QGraphicsView;
    grid3 = new QGraphicsView;
    grid4 = new QGraphicsView;

    QHBoxLayout *lay1 = new QHBoxLayout;
    lay1->addWidget(grid1);
    lay1->addSpacing(5);
    lay1->addWidget(grid2);

    QHBoxLayout *lay2 = new QHBoxLayout;
    lay2->addWidget(grid3);
    lay2->addSpacing(5);
    lay2->addWidget(grid4);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(lay1);
    layout->addSpacing(5);
    layout->addLayout(lay2);
//    layout->co
}



