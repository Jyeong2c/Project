#include "scene.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsItem>
#include <QPixmap>


#define ItemWidth   4


Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{
    m_currentItem = nullptr;
    m_currentShape = Cursor;
    m_isClicked = false;
    m_imCount = 1;


//    slot_11();      // 임플란트
}


void Scene::slot_11()
{
//    QString imagePath(tr(":/teeth/teeth/topLeft/11.png"));
//    QPixmap* teeth11 = new QPixmap(imagePath);
//    showTeeth = teeth11;

//    qDebug() << "2222222";
}

void Scene::slot_12()
{
//    QString imagePath(tr(":/teeth/teeth/topLeft/12.png"));
//    QPixmap* teeth12 = new QPixmap(imagePath);
//    showTeeth = teeth12;

//    qDebug() << "333333";
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_currentShape == Implant){
        if(!m_isClicked){
            m_prevPos = event->scenePos();      // m_prevPos : 이전 좌표값
            m_isClicked = true;

            QBrush brush(QColor(Qt::red), Qt::SolidPattern);
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setBrush(brush);
            item1->setRect(0, 0, ItemWidth, ItemWidth);
            item1->setPos(m_prevPos);
            addItem(item1);
            m_point.append(item1);
        }
        else {
            m_latePos = event->scenePos();      // m_latePos : 현재 좌표값
            addImplantItem(m_prevPos, m_latePos);
            m_isClicked = false;
            //            m_currentShape = Cursor;

            /* 클릭했을 때 좌표 점 찍기 */
            QBrush brush(QColor(Qt::blue), Qt::SolidPattern);
            QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem();
            item2->setBrush(brush);
            item2->setRect(0, 0, ItemWidth, ItemWidth);
            item2->setPos(m_latePos);
            addItem(item2);
            m_point.append(item2);
        }
        QGraphicsScene::mousePressEvent(event);
    }
}



//void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(m_currentShape == Implant){
//        if(m_imCount == 1){
//            m_prevPos = event->scenePos();      // m_prevPos : 이전 좌표값
////            m_isClicked = true;
//            m_imCount++;

//            QBrush brush(QColor(Qt::red), Qt::SolidPattern);
//            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
//            item1->setBrush(brush);
//            item1->setRect(0, 0, ItemWidth, ItemWidth);
//            item1->setPos(m_prevPos);
//            addItem(item1);
//            m_point.append(item1);

//        }
//        else if(m_imCount == 2) {
//            m_latePos = event->scenePos();      // m_latePos : 현재 좌표값
////            addImplantItem(m_prevPos, m_latePos);
//            m_isClicked = false;
////            m_currentShape = Cursor;
//            m_imCount++;


//            /* 클릭했을 때 좌표 점 찍기 */
//            QBrush brush(QColor(Qt::blue), Qt::SolidPattern);
//            QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem();
//            item2->setBrush(brush);
//            item2->setRect(0, 0, ItemWidth, ItemWidth);
//            item2->setPos(m_latePos);
//            addItem(item2);
//            m_point.append(item2);
//        }
//        else if(m_imCount == 3) {
//            m_sPos = event->screenPos();
//            addImplantItem(m_prevPos, m_latePos, m_sPos);
//            m_imCount = 1;
//        }
//    }
//    QGraphicsScene::mousePressEvent(event);
//}


void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* 선택된 아이템 이동 */

    foreach(auto item, selectedItems())
        item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
    update();

    QGraphicsScene::mouseMoveEvent(event);
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

    QString imagePath(tr(":/teeth/teeth/topLeft/11.png"));
    QPixmap teeth11(imagePath);
    pixmapItem = new QGraphicsPixmapItem(teeth11.scaled(rect.normalized().size().toSize(), Qt::IgnoreAspectRatio));

    pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable);
    pixmapItem->setPos(stPos);

    addItem(pixmapItem);

    m_implantItems.append(pixmapItem);
    qDebug()<< m_implantItems.size();
}


//void Scene::addImplantItem(QPointF stPos, QPointF edPos, QPoint mPos)
//{
//    QRectF rect(stPos, edPos);
//    rect.translate(center)

////    QRectF rect;
////    rect.setTopLeft(stPos);
////    rect.setBottomLeft(edPos);
////    rect.setWidth((mPos.x()-((stPos.x() + edPos.x())/2)));




//    QString imagePath(tr(":/teeth/teeth/topLeft/11.png"));
//    QPixmap teeth11(imagePath);
//    pixmapItem = new QGraphicsPixmapItem(teeth11.scaled(v.size(), Qt::IgnoreAspectRatio));

//    pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable);
//    pixmapItem->setPos(stPos);

//    addItem(pixmapItem);

//    m_implantItems.append(pixmapItem);
//    qDebug()<< m_implantItems.size();
//}




void Scene::setCurrentShape(const Shape &value)
{
    m_currentShape = value;
}

void Scene::clearItems()
{
    foreach(auto items, m_implantItems)
        removeItem(items);
}



/* 패닝 */
//void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
////    if (event->button() == Qt::RightButton)
////    {
////        _pan = true;
////        _panStartX = event->pos().x();
////        _panStartY = event->pos().y();
//////        setCursor(Qt::ArrowCursor);
////        event->accept();
////        return;
////        qDebug() << "오른쪽 패닝 클릭";
////    }
////    event->ignore();
//    qDebug() << "오른쪽 패닝 클릭";
//}

//void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
////    if (event->button() == Qt::RightButton)
////    {
////        _pan = false;
//////        setCursor(Qt::ArrowCursor);
////        event->accept();
////        return;
////        qDebug() << "오른쪽 패닝 뗌";
////    }
////    event->ignore();
//    qDebug() << "오른쪽 패닝 뗌";
//}

//void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
//    emit sig_move(event);
//}
