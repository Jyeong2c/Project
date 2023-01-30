#include "view.h"


#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsView>

View::View(QWidget *parent)
    : QGraphicsView{parent}
{

}

void View::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        qDebug() << "제발 찍혀라";

    }

    if (event->button() == Qt::RightButton)
    {
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();   
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        _pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if (_pan)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}
