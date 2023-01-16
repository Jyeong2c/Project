#include "fmx.h"

#include <QGridLayout>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QSpacerItem>

FMX::FMX(QWidget *parent)
    : QWidget{parent}
{
    fmx();
}

/* 14 FMX */
void FMX::fmx()
{
    QGraphicsView *fmx1 = new QGraphicsView;
    QGraphicsView *fmx2 = new QGraphicsView;
    QGraphicsView *fmx3 = new QGraphicsView;
    QGraphicsView *fmx4 = new QGraphicsView;
    QGraphicsView *fmx5 = new QGraphicsView;
    QGraphicsView *fmx6 = new QGraphicsView;
    QGraphicsView *fmx7 = new QGraphicsView;
    QGraphicsView *fmx8 = new QGraphicsView;
    QGraphicsView *fmx9 = new QGraphicsView;
    QGraphicsView *fmx10 = new QGraphicsView;
    QGraphicsView *fmx11 = new QGraphicsView;
    QGraphicsView *fmx12 = new QGraphicsView;
    QGraphicsView *fmx13 = new QGraphicsView;
    QGraphicsView *fmx14 = new QGraphicsView;

    /* 왼쪽 2 by 2 */
    QHBoxLayout *lay1 = new QHBoxLayout;
    lay1->addWidget(fmx1);
    lay1->addSpacing(5);
    lay1->addWidget(fmx2);


    QHBoxLayout *lay2 = new QHBoxLayout;
    lay2->addWidget(fmx8);
    lay2->addSpacing(5);
    lay2->addWidget(fmx9);

    QVBoxLayout *sumlay1 = new QVBoxLayout;
    sumlay1->addLayout(lay1);
    sumlay1->addSpacing(130);
    sumlay1->addLayout(lay2);

    /* 가운데 3 by 3 */
    QHBoxLayout * lay3 = new QHBoxLayout;
    lay3->addWidget(fmx3);
    lay3->addSpacing(5);
    lay3->addWidget(fmx4);
    lay3->addSpacing(5);
    lay3->addWidget(fmx5);

    QHBoxLayout * lay4 = new QHBoxLayout;
    lay4->addWidget(fmx10);
    lay4->addSpacing(5);
    lay4->addWidget(fmx11);
    lay4->addSpacing(5);
    lay4->addWidget(fmx12);

    QVBoxLayout *sumlay2 = new QVBoxLayout;
    sumlay2->addLayout(lay3);
    sumlay2->addSpacing(35);
    sumlay2->addLayout(lay4);

    /* 오른쪽 2 by 2 */
    QHBoxLayout *lay5 = new QHBoxLayout;
    lay5->addWidget(fmx6);
    lay5->addSpacing(5);
    lay5->addWidget(fmx7);

    QHBoxLayout *lay6 = new QHBoxLayout;
    lay6->addWidget(fmx13);
    lay6->addSpacing(5);
    lay6->addWidget(fmx14);

    QVBoxLayout *sumlay3 = new QVBoxLayout;
    sumlay3->addLayout(lay5);
    sumlay3->addSpacing(130);
    sumlay3->addLayout(lay6);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addLayout(sumlay1, 3);
    layout->addSpacing(5);
    layout->addLayout(sumlay2, 4);
    layout->addSpacing(5);
    layout->addLayout(sumlay3, 3);
}
