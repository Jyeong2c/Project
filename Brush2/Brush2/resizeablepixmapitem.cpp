#include "resizeablepixmapitem.h"
#include <QPen>
#include <QGraphicsScene>
#include <QPainter>

ResizeablePixmapItem::ResizeablePixmapItem(QPixmap pixmap):
    mPixmap(pixmap)
{
    setRect (QRectF(10,10,300,300));
    setOwnerItem(this);
}


QRectF ResizeablePixmapItem::selectorFrameBounds() const
{
    return  rect();
}

void ResizeablePixmapItem::setSelectorFrameBounds(QRectF boundsRect)
{

    prepareGeometryChange();
    setRect(boundsRect);
    update();


}

QRectF ResizeablePixmapItem::boundingRect() const
{
    return selectorFrameBounds();
}

void ResizeablePixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();

//    painter->setBrush(brush());

//    painter->drawRect(rect());
    painter->drawPixmap(boundingRect(),mPixmap,mPixmap.rect());

    drawHandlesIfNecessary();

    painter->restore();
}





QPixmap ResizeablePixmapItem::getPixmap() const
{
    return mPixmap;
}

void ResizeablePixmapItem::setPixmap(const QPixmap &value)
{
    mPixmap = value;
}
