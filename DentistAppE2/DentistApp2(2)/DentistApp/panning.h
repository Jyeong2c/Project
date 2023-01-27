#ifndef PANNING_H
#define PANNING_H

#include <QGraphicsEllipseItem>

class panning :public QGraphicsEllipseItem
{
public:
    panning(QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF m_offset;
    bool m_isMovable;
};

#endif // PANNING_H
