#include "view.h"

#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsView>

View::View(QWidget *parent)
    : QGraphicsView{parent}
{

}

/* 마우스를 클릭했을 때 실행하는 함수 override */
void View::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        _pan = true;
        _panStartX = event->x();    // 클릭한 좌표의 x값
        _panStartY = event->y();    // 클릭한 좌표의 y값
        setCursor(Qt::ArrowCursor); // 커서 모양 설정
    }
    QGraphicsView::mousePressEvent(event);
}

/* 마우스를 눌렀다가 뗐을 때 실행하는 함수 override */
void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        _pan = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

/* 마우스를 누르고 움직였을 때 실행하는 함수 override - 이미지 패닝 */
void View::mouseMoveEvent(QMouseEvent *event)
{
    if (_pan == true)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsView::mouseMoveEvent(event);
}
