#ifndef MYPOLY_H
#define MYPOLY_H

#include <QGraphicsItem>

class MyPoly : public QGraphicsItem
{
public:
    MyPoly();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPoint startPoint;
    QLine m_line;
    bool pressed = false;
};

#endif // MYPOLY_H
