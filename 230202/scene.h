#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    enum Shape {Cursor, Implant};

        void setCurrentShape(const Shape &value);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Shape m_currentShape;
    QGraphicsItem* m_currentItem;

    bool m_isClicked;
    QPointF m_prevPos;  // 이전 좌표값
    QPointF m_latePos;  // 이후 좌표값

    void addImplantItem(QPointF stPos, QPointF edPos);          // 마우스 위치에 임플란트 이미지를 출력하는 함수

    QList<QGraphicsItem*> m_allitems;



signals:

};

#endif // SCENE_H
