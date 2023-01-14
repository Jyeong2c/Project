#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Scene : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject, DrawLine};
    Scene(QObject* parent = 0);
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Mode sceneMode;
    QPointF origPoint;
    QGraphicsLineItem* itemToDraw;
    //void makeItemsControllable(bool areControllable);
};



#endif // SCENE_H
