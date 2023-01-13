#include "layout.h"

#include <QGridLayout>
#include <QGraphicsView>

Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();
}

/* 2 X 2 Grid */
void Layout::gridLayout()
{
    //QWidget *window = new QWidget;

    QGraphicsView *grid1 = new QGraphicsView;
    QGraphicsView *grid2 = new QGraphicsView;
    QGraphicsView *grid3 = new QGraphicsView;
    QGraphicsView *grid4 = new QGraphicsView;

//    QGridLayout *layout = new QGridLayout(this);
//    layout->addWidget(grid1, 0, 0);                   // QWidget*, row, column->addSpacing(5);
//    layout->addWidget(grid2, 0, 1);
//    layout->addWidget(grid3, 1, 0);
//    layout->addWidget(grid4, 1, 1);

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

    //layout->addWidget(bigButton, 2, 0, 1, 2);
}
