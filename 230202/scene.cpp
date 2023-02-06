#include "scene.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsItem>

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{
    m_currentShape = Cursor;
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_currentShape == Implant){
        if(!m_isClicked){
            m_prevPos = event->scenePos();      // m_prevPos : 이전 좌표값
            m_isClicked = true;
        }
        else{
            m_latePos = event->scenePos();      // m_latePos : 현재 좌표값
            addImplantItem(m_prevPos, m_latePos);
            m_isClicked = false;
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_currentItem != nullptr)
        delete m_currentItem;

    switch(m_currentShape){

    case Cursor:
        break;

    case Implant:
        break;
    }
    m_currentItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::addImplantItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QPixmap pixmap("51.png");
//    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap.scaled(20, 40, Qt::KeepAspectRatio));
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap.scaled(rect.size().toSize(), Qt::KeepAspectRatio));

    pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable);
    pixmapItem->setPos(stPos);

    addItem(pixmapItem);

    m_allitems.append(pixmapItem);
    qDebug()<< m_allitems.size();
}

void Scene::setCurrentShape(const Shape &value)
{
    m_currentShape = value;

}
