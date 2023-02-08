#include "maxlayout.h"
#include "view.h"
//#include "qevent.h"

#include <QGraphicsView>
#include <QBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>

Maxlayout::Maxlayout(QWidget *parent)
    : QWidget{parent}
{
    maxWidget = new QWidget();
    maxNewGrid = new View;
    maxLayBox = new QHBoxLayout(this);
    maxLayBox->addWidget(maxNewGrid);
    maxLayBox->setContentsMargins(0,0,0,0);
    maxNewSc = new QGraphicsScene;
    maxNewGrid->setScene(maxNewSc);

    qDebug() << "newGrid size1" << maxNewGrid->viewport()->size();

    viewQuit = new QPushButton("X", this);
    viewQuit->setStyleSheet("background-color: rgb(231, 230, 230);"
                            "border : 2px solid rgb(0, 0, 0);");
}

void Maxlayout::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    qDebug() << "gridWidget changed";

//    if(event->oldSize().width() > 0){
//        maxNewGrid->scale(event->size().width()/(float)event->oldSize().width(),
//                          event->size().height()/(float)event->oldSize().height());
//    }

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

void Maxlayout::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        DPointLeft = event->pos();
        emit sig_leftDClicked(DPointLeft);
    }

    if(event->button() == Qt::MiddleButton) {
        clickPoint = event->pos();
        emit sig_clicked(clickPoint);
    }
}

