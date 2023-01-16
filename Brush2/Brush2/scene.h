#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Scene: public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);

    void setPaintColor();
//    void setPaintSize(qreal);
    QColor paintColor;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:
    void drawLineTo(const QPointF &endPoint);


    bool drawing;
    QGraphicsItemGroup * lineGroup;
    QPointF startingPoint;
    QPointF lastPenPoint;
    qreal paintSize;

public slots:
        void setPaintSize(qreal);

};

#endif // SCENE_H
