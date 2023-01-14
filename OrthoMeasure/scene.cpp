#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsLineItem>
#include <QGraphicsView>

Scene::Scene(QObject* parent)
{

}
void Scene::setMode(Mode mode)
{
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == DrawLine){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(sceneMode == DrawLine){
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem(0);
            itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0, 0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    itemToDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event) {

}
