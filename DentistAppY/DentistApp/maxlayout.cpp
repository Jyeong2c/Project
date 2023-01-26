#include "maxlayout.h"

#include <QGraphicsView>
#include <QBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPushButton>
Maxlayout::Maxlayout(QWidget *parent)
    : QWidget{parent}
{
    maxWidget = new QWidget();
    maxNewGrid = new QGraphicsView;
    maxLayBox = new QHBoxLayout(this);
    maxLayBox->addWidget(maxNewGrid);
    maxLayBox->setContentsMargins(0,0,0,0);
    maxNewSc = new QGraphicsScene;
    //maxNewSc->setBackgroundBrush(Qt::yellow);
    maxNewGrid->setScene(maxNewSc);

    qDebug() << "newGrid size1" << maxNewGrid->viewport()->size();

    viewQuit = new QPushButton("X", this);
//    viewQuit->setGeometry(905, 10, 30, 30);
    viewQuit->setStyleSheet("background-color: rgb(255, 255, 255);"
                            "border : 2px solid rgb(0, 0, 0);");
}

void Maxlayout::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    qDebug() << "gridWidget changed";

    emit max_sig_size(maxNewGrid);
}

void Maxlayout::setNewSc(QGraphicsScene* sc)
{
    qDebug() << "maxNewGrid changed";
    maxNewSc = sc;
    maxNewGrid->setScene(maxNewSc);
}

void Maxlayout::setNewGrid(QGraphicsView* view)
{
    maxNewGrid = view;
}



