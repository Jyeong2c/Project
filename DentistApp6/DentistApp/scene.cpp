#include "scene.h"

Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    itemToDraw = 0;
    //emit OrthoSend(1, 1);
}


void Scene::setMode(Mode mode){
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

/*이벤트 핸들러를 재정의하는 것 외에도 장면 모드에 따라 항목의 선택 가능성과 이동성을 조정하는 도우미 기능 makeItemsControlable(boole)도 도입한다.*/
void Scene::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
    }
}

/*라인의 시작점을 지정 이 점을 QPointForigPos에 저장*/
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);

    /*QGraphicsScene상에 mouse의 버튼을 누를 시 시작위치를 계산하는 디버깅*/
    qDebug() << "start x : " << event->scenePos().x() <<  ", " << "start y : " << event->scenePos().y();
//    int x = event->scenePos().x();
//    int y = event->scenePos().y();
    emit OrthoSend(event->scenePos().x(), event->scenePos().y());
}

/*QGraphicsLineItem의 인스턴스가 이미 존재하는지 확인*/
/*그렇지 않은 경우 새 인스턴스를 호출 그 위치를 origPos로 설정(이 과정은 한번밖에 안함)*/
/*mouseMoveEvent()에 대한 모든 후속 호출에서  QGraphicsLineItem을 정의하는 경계선을 업데이트한다.*/
/*경계 선은 항상 (0,0)에서 시작하며, 끝점은 이벤트->scenePos()와 origPoint 사이의 거리에 따라 결정됩니다.
 * QGraphicsLineItem의 실제 위치는 'if 블록'에서 setPos(QPointF)로 표시되었습니다.*/
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine){
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem;
            this->addItem(itemToDraw);
            itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

/*mouseReleaseEvent()에서는 itemToDraw포인터를 0으로 설정하여 그리기를 종료*/
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    itemToDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);

    sceneMode = SelectObject;
    /*QGraphicsScene상에 mouse에 버튼을 올렸을 때 마지막위치를 계산하는 디버깅*/
    qDebug() << "end x : " << event->scenePos().x() <<  ", " << "end y : " << event->scenePos().y();
    emit OrthoSend(event->scenePos().x(), event->scenePos().y());
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else
        QGraphicsScene::keyPressEvent(event);
}
