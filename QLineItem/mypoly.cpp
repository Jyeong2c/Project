#include "mypoly.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

MyPoly::MyPoly()
{

}

void MyPoly::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(Qt::darkBlue));
    painter->setPen(pen);
    painter->drawRect(boundingRect().adjusted(5, 5, -5, -5));

    if(m_line.isNull()) return;
    pen.setWidth(10);
    pen.setColor(QColor(Qt::red));
    painter->setPen(pen);
    painter->drawLine(m_line);
}
QRectF MyPoly::boundingRect() const{
    return QRectF(QPointF(0, 0), QPointF(200, 200));
}
void MyPoly::mousePressEvent(QGraphicsSceneMouseEvent *event){
    startPoint = event->scenePos().toPoint();
    qDebug() << "Pressed, start: " << startPoint;
    pressed = true;
    update();
    //QGraphicsItem::mousePressEvent(event);
}
void MyPoly::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(pressed){
        QPoint endPoint = event->scenePos().toPoint();
        qDebug() << "Moving, start : " << startPoint << " end: " << endPoint;
        m_line.setPoints(startPoint, endPoint);
        update();
    }
    QGraphicsItem::mousePressEvent(event);
}
void MyPoly::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    pressed = false;
    QPoint endPoint = event->scenePos().toPoint();
    m_line.setPoints(startPoint, endPoint);
    qDebug() << "Release, start: " << startPoint << " end: " << endPoint;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

