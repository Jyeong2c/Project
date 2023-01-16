#include "scene.h"
//#include "resizeablepixmapitem.h"


#include <QGraphicsItem>

#include <QGraphicsSceneMouseEvent>
#include <QColorDialog>
#include <QDebug>

Scene::Scene(QObject *parent) : QGraphicsScene(parent),drawing(false)
{
    paintColor = Qt::green;


}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){

        startingPoint = event->scenePos();
        // qDebug()<<startingPoint;

        drawing = true;
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && drawing){
        drawLineTo(event->scenePos());
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if((event->button() == Qt::LeftButton) && drawing){
        //lineGroup = nullptr;
        drawing = false;

    }

}


void Scene::drawLineTo(const QPointF &endPoint)
{
    if(lineGroup != nullptr){                   // 드로잉하고 있을 때
        lineGroup = new QGraphicsItemGroup();
        lineGroup->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        addItem(lineGroup);
        lastPenPoint = endPoint;
    }


    QGraphicsLineItem  *localLine = new QGraphicsLineItem(QLineF(lastPenPoint,endPoint));
    QPen mPen;
    mPen.setWidth(3);
    //mPen.setCapStyle(Qt::RoundCap);
    mPen.setColor(paintColor);
    localLine->setPen(mPen);
    lineGroup->addToGroup(localLine);
    lastPenPoint = endPoint;

}

void Scene::setPaintColor(){

    paintColor = QColorDialog::getColor(paintColor);

}

void Scene::setPaintSize(qreal size){
    paintSize = size;
}
