#ifndef RESIZEABLEPIXMAPITEM_H
#define RESIZEABLEPIXMAPITEM_H

#include <QGraphicsRectItem>
#include "resizeablehandlerect.h"

class ResizeablePixmapItem: public QGraphicsRectItem,public ResizeableHandleRect
{
public:
    ResizeablePixmapItem(QPixmap pixmap);

    QRectF selectorFrameBounds() const;
    void setSelectorFrameBounds(QRectF boundsRect);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPixmap getPixmap() const;
    void setPixmap(const QPixmap &value);

private:
    QPixmap mPixmap;
};

#endif // RESIZEABLEPIXMAPITEM_H
