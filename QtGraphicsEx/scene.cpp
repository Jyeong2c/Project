#include "scene.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

Scene::Scene()
{

}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->button() != Qt::LeftButton)
        return;

    line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
    line->setPen(QPen(Qt::black, 2));

    addItem(line);

    QGraphicsScene::mousePressEvent(mouseEvent);
}
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(line != nullptr){
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    Scene::mouseMoveEvent(mouseEvent);
}
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(line != nullptr){
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if(startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if(endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
    }
    line = nullptr;
    Scene::mouseReleaseEvent(mouseEvent);
}
