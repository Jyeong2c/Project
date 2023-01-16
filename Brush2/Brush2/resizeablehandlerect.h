#ifndef RESIZEABLEHANDLERECT_H
#define RESIZEABLEHANDLERECT_H

#include <QRectF>
#include <QPainter>
#include <QGraphicsItem>


class ResizeableHandleRect
{
public:
    ResizeableHandleRect();
    virtual ~ ResizeableHandleRect();

    virtual QRectF selectorFrameBounds() const = 0;
    virtual void setSelectorFrameBounds(QRectF boundsRect) = 0;

    void drawHandlesIfNecessary();

    QPixmap getPixmap() const;
    void setPixmap(const QPixmap &value);

    void setOwnerItem(QGraphicsItem *value);

private:
    void drawHandles();
    QRectF topleftHandleRect;
    QRectF topRightHandleRect;
    QRectF bottomRightHandleRect;
    QRectF bottomLeftHandleRect;

   // QList<HandleItem * > handleList;
    bool handlesAddedToScene;
    QGraphicsItem * ownerItem;
};

#endif // RESIZEABLEHANDLERECT_H
