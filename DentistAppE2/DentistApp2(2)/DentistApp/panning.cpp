#include "panning.h"

#include <QGraphicsSceneMouseEvent>
#include <QBrush>

panning::panning(QGraphicsItem *parent) : QGraphicsEllipseItem(parent), m_isMovable(false)
{
    QBrush brush(QColor(Qt::black), Qt::SolidPattern);
    setBrush(brush);
    setRect(0, 0, 16, 16);
}

void panning::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_isMovable = true;
    m_offset = event->pos();
    setPos(mapToScene(event->pos().rx()-8, event->pos().ry()-8));
    QGraphicsItem::mousePressEvent(event);
    event->accept();
}

void panning::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isMovable) {
        setPos(mapToScene(event->pos().rx()-8, event->pos().ry()-8));
    }
    QGraphicsItem::mouseMoveEvent(event);
    event->ignore();
}

void panning::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && m_isMovable) {
        m_isMovable = false;
        update();
    }
    QGraphicsItem::mouseReleaseEvent(event);
    event->ignore();
}

