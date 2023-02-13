#include "scene.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsItem>
#include <QPixmap>
#include <cmath>
#include <QVector2D>

/* EUN JI */
//--------------------
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QKeyEvent>
#include <QTransform>
#include <QDebug>
//--------------------


#define ItemWidth   4


Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}, drawing(false), m_currentColor(Qt::red), paintSize(3.0f), rotateAngle(0.0)
{
    m_currentItem = nullptr;
    m_currentShape = Cursor;
    m_isClicked = false;
    m_imCount = 1;
    m_triClicked = 1;           // EUN JI


/* JAE HY */
//-------------------------------------------------------
    /*길이 측정을 위한 두점을 Scene클래스 내에서 처리하기 위한 커넥트*/
    connect(this, &Scene::sendFirstOrtho, &Scene::reFirstOrtho);
    connect(this, &Scene::sendLastOrtho, &Scene::reLastOrtho);

    /*connect 모음 (각도를 측정하기 위한 세점의 좌표를 보내는 신호와 슬롯을 연결*/
    connect(this, &Scene::firstAnglePoint, &Scene::reFirstAnglePoint);
    connect(this, &Scene::secondAnglePoint, &Scene::reSecondAnglePoint);
    connect(this, &Scene::thirdAnglePoint, &Scene::reThirdAnglePoint);
//---------------------------------------------------------
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
/* EUN JI*/
//--------------------------------------------------------------
    if(event->button() == Qt::LeftButton) {
        QPen pen(m_currentColor, paintSize);
        if(m_currentShape == Path) {
            drawing = true;

            QPainterPath path;
            QGraphicsPathItem* item = addPath(path);
            item->setPen(pen);
            item->setBrush(Qt::transparent);

            path = item->path();
            path.moveTo(event->scenePos());
            item->setPath(path);
            m_pathList.append(item);
        }
        else if(m_currentShape == Line) {
            if(!m_isClicked){
                m_prevPos = event->scenePos();
                m_isClicked = true;
            }
            else {
                m_latePos = event->scenePos();
                addLineItem(m_prevPos, m_latePos);
                m_isClicked = false;
            }
        }
        else if(m_currentShape == Rect) {
            if(!m_isClicked){
                m_prevPos = event->scenePos();
                m_isClicked = true;
            }
            else {
                m_latePos = event->scenePos();
                addRectItem(m_prevPos, m_latePos);
                m_isClicked = false;
            }
        }

        //---------------------------------------------------------------
        /* YUNA*/
        else if(m_currentShape == Implant) {
            if(m_imCount == 1) {
                m_prevPos = event->scenePos();      // m_prevPos : 첫 번째 좌표값 (Top Left)
                m_imCount++;

                /* 클릭했을 때 좌표 점 찍기 (빨간점) */
//                QBrush brush(QColor(Qt::red), Qt::SolidPattern);
//                QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
//                item1->setBrush(brush);
//                item1->setRect(0, 0, ItemWidth, ItemWidth);
//                item1->setPos(m_prevPos);
//                addItem(item1);
//                m_point.append(item1);
            }

            else if(m_imCount == 2) {
                m_latePos = event->scenePos();      // m_latePos : 두 번째 좌표값 (Bottom Left)
                m_imCount++;

                /* 클릭했을 때 좌표 점 찍기 (파란점) */
//                QBrush brush(QColor(Qt::blue), Qt::SolidPattern);
//                QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem();
//                item2->setBrush(brush);
//                item2->setRect(0, 0, ItemWidth, ItemWidth);
//                item2->setPos(m_latePos);
//                addItem(item2);
//                m_point.append(item2);
            }

            else if(m_imCount == 3) {
                m_sPos = event->scenePos();      // m_sPos : 세 번째 좌표값 (Right의 임의의 한 점의 좌표값 = width의 역할)
                addImplantItem(m_prevPos, m_latePos, m_sPos);
                m_imCount = 1;
                m_currentShape = Cursor;          // 1개의 임플란트만 나오게 설정하는 부분

                /* 클릭했을 때 좌표 점 찍기 (노란점) */
//                QBrush brush(QColor(Qt::yellow), Qt::SolidPattern);
//                QGraphicsEllipseItem* item3 = new QGraphicsEllipseItem();
//                item3->setBrush(brush);
//                item3->setRect(0, 0, ItemWidth, ItemWidth);
//                item3->setPos(m_sPos);
//                addItem(item3);
//                m_point.append(item3);
            }
        }
//---------------------------------------------------------------
        else if(m_currentShape == Ellipse) {
            drawEllipse = true;                     // 왼쪽 마우스 bool 타입선언
            m_startPos = event->scenePos();
        }
        else if(m_currentShape == Arrow) {
            if(!m_isClicked) {
                m_prevPos =event->scenePos();
                m_isClicked = true;
            }
            else {
                m_latePos = event->scenePos();
                addArrowItem(m_prevPos, m_latePos);
                m_isClicked = false;
            }
        }
        else if(m_currentShape == Triangle) {
            if(m_triClicked == 1) {
                m_prevPos = event->scenePos();
                m_triClicked++;

            } else if(m_triClicked == 2) {
                m_midPos = event->scenePos();
                m_triClicked++;

            } else if(m_triClicked == 3) {
                m_latePos = event->scenePos();
                addTriangleItem(m_prevPos, m_midPos, m_latePos);
                m_triClicked = 1;
            }
        }
        else if(m_currentShape == Text) {
            m_prevPos = event->scenePos();
            addTextItem(m_prevPos);
        }

        /* JAE HY */
        //------------------------------------------------------------------------------
        else if(m_currentShape == Length) {
            if(event->button() == Qt::LeftButton) {
                if(!m_isClicked) {
                    emit sendFirstOrtho(event->scenePos().x(), event->scenePos().y());
                    m_prevPos = event->scenePos();
                    m_isClicked = true;
                }
                else {
                    pointCount++;
                    m_latePos = event->scenePos();
                    addLengthItem(m_prevPos, m_latePos);
                    m_isClicked = false;
                }
            }
        }
        else if(m_currentShape == Angle) {
            if(event->button() == Qt::LeftButton) {
                if(m_triClicked == 1) {
                    emit firstAnglePoint(event->scenePos().x(), event->scenePos().y());
                    m_prevPos = event->scenePos();
                    m_triClicked++;

                } else if(m_triClicked == 2) {
                    emit secondAnglePoint(event->scenePos().x(), event->scenePos().y());
                    m_midPos = event->scenePos();
                    m_triClicked++;

                } else if(m_triClicked == 3) {
                    emit thirdAnglePoint(event->scenePos().x(), event->scenePos().y());
                    m_latePos = event->scenePos();

                    qDebug() << "first x, y : " << fstPosX << " , " << fstPosY;
                    qDebug() << "second x, y : " << sedPosX << " , " << sedPosY;
                    qDebug() << "third x, y : " << trdPosX << " , " << trdPosY;

                    /*3점의 좌표를 지정하면 해당 좌표의 1,2 2,3 1,3 점끼리 선분을 생성*/
                    int a = qSqrt(qPow((sedPosX - fstPosX), 2) + qPow((sedPosY - fstPosY), 2));
                    qDebug() << "a : " << a;
                    int b = qSqrt(qPow((trdPosX - sedPosX), 2) + qPow((trdPosY - sedPosY), 2));
                    qDebug() << "b : " << b;
                    int c = qSqrt(qPow((fstPosX - trdPosX), 2) + qPow((fstPosY - trdPosY), 2));
                    qDebug() << "c : " << c;

                    /*세 점의 좌표를 찍으면 2번째 사이각의 각도를 구하는 공식*/
                    /*그러나 현 각도는 180도 내에서만 각도를 측정할 수 밖에 없음.*/
                    auto ang = qAcos((qPow(a, 2) + qPow(b, 2) - qPow(c, 2)) / (2 * a * b)) * 180 / 3.141592;
                    qDebug() << "Angle : " << ang;

                    emit sendAngleMeasure(round(ang));
                    addAngleItem(m_prevPos, m_midPos, m_latePos);
                    m_triClicked = 1;
                }
            }
        }
        //------------------------------------------------------------------------------
    }

    else if(m_currentShape != Cursor) {
        m_startPos = event->scenePos();
    }
    QGraphicsScene::mousePressEvent(event);
}


void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
/* EUN JI*/
//--------------------------------------------------------------
    if(m_currentShape == Path) {
        if(drawing) {
            QGraphicsPathItem* item = m_pathList.last();
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            if(item) {

                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
        }
    }
    else if(m_currentShape == Cursor) {
        /* 선택된 아이템 이동 */
        foreach(auto item, selectedItems())
            item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
        update();
    }

    //--------------------------------------------------------------
    /* YUNA */
    else {
        /* 선택된 아이템 이동 */
        foreach(auto item, selectedItems())
            item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
        update();
    }
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

/* EUN JI*/
//----------------------------------------------------------------
    case Line:
        break;

    case Rect:
        break;

    case Ellipse:
        if(event->button() == Qt::LeftButton) {
            drawEllipse = false;
            addEllipseItem(m_startPos, event->scenePos());
            break;
        }

    case Path:
        if(drawing){
            QGraphicsPathItem* item = m_pathList.last();

            if(item){
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
            drawing = false;
        }
        break;

    case Arrow:
        break;

    case Triangle:
        break;

    case Text:
        m_currentShape = Cursor;
        break;

    /* JAE HY */
    //---------------------------------------------------------------------------
    case Length:
    {
        if(pointCount == 1) {
            /*길이 아이템이 선택되는 모드로 설정*/
            int sceneWidth = sceneRect().topRight().x() - sceneRect().topLeft().x();
            int sceneHeight = sceneRect().bottomRight().y() - sceneRect().topRight().y();
            qDebug() << "sceneWidth : " << sceneWidth;
            qDebug() << "sceneHeight : " << sceneHeight;
            emit sendLastOrtho(event->scenePos().x(), event->scenePos().y());
            /*서버의 이미지 크기도 여기에서 수정되어야 함.*/
            double avgWidth = (double)400 / sceneWidth;
            double avgHeight = (double)200 / sceneHeight;

            double leng = qSqrt(qPow((lengthLastX - lengthFstX) * avgWidth, 2) +
                                qPow((lengthLastY - lengthFstY) * avgHeight, 2));
            qDebug() << "Length : " <<  leng << "cm";

            emit sendLengthMeasure(leng);
            pointCount = 0;
        }
        break;
    }
    case Angle:
        break;
//---------------------------------------------------------------------------
    }

//----------------------------------------------------------------
    m_currentItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}



QColor Scene::getCurrentColor() const
{
    return m_currentColor;
}

void Scene::setCurrentColor(const QColor& color)
{
    m_currentColor = color;
}
//---------------------------------------------------------------

void Scene::setCurrentShape(const Shape &value)
{
    m_currentShape = value;
}

void Scene::addImplantItem(QPointF stPos, QPointF edPos, QPointF mPos)
{
//    int height = sqrt(pow(edPos.x() - stPos.x(), 2) + pow(edPos.y() - edPos.y(), 2));
//    int width = sqrt(pow(mPos.x()-stPos.x(), 2) + pow(mPos.y() - edPos.y(), 2));
//    QSizeF size(width, height);

    qDebug() << stPos;          // 첫 번째 좌표
    qDebug() << edPos;          // 두 번째 좌표
    qDebug() << mPos;           // 세 번째 좌표

    int xPos = (edPos.x() + stPos.x())/2;
    int yPos = (edPos.y() + stPos.y())/2;

    int line = sqrt(pow(xPos - mPos.x(), 2) + pow(yPos - mPos.y(), 2));

    QVector2D dot1(stPos);
    QVector2D dot2(edPos);
//    QVector2D dot3(mPos);

    float distance1 = dot1.distanceToPoint(dot2);
//    float distance2 = dot1.distanceToPoint(dot3);

    QSizeF size(line, distance1);
    QRectF rect(stPos, size);

    qDebug() << distance1;
//    qDebug() << distance2;
    qDebug() << line;


    //    double angle = atan2((stPos.y()-edPos.y()), (stPos.x()-edPos.x()));

#if 0
    /* 시그널 슬롯을 사용하지 않고 이미지 경로로 바로 접근해서 잘 되는 경우 */
    QString imagePath(tr(":/teeth/teeth/topLeft/11.png"));
    QPixmap teeth11(imagePath);
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(teeth11.scaled(rect.normalized().size().toSize(), Qt::IgnoreAspectRatio));
#else

    qDebug() << "addImplantItem의 중간 위치" << showTeeth;
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(showTeeth->scaled(rect.normalized().size().toSize(), Qt::IgnoreAspectRatio));
//    pixmapItem = new QGraphicsPixmapItem(showTeeth->scaled(rect.normalized().size().toSize(), Qt::IgnoreAspectRatio));
//    implantHash[pixmapItem];

#endif

    pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable);
    pixmapItem->setPos(stPos);

//    double angle = atan2(rect.y(), rect.x());
//    double angle = atan2((stPos.y()-edPos.y()), (stPos.x()-edPos.x()));

//    double angleRadian = atan((edPos.y() - stPos.y()) / (edPos.x() - stPos.x()));


    double angleRadian = atan2((stPos.x()-edPos.x()), (stPos.y()-edPos.y()));
    double angleDegree = floor(qRadiansToDegrees(angleRadian));
    pixmapItem->setRotation(180 - angleDegree);


//    if(mPos.x() > edPos.x()) {
//        pixmapItem->setRotation(180 - angleDegree);
//    }
//    else if(mPos.x() < edPos.x()) {
//        pixmapItem->setRotation(angleDegree - 180);
//    }

    qDebug() << angleRadian;
    qDebug() << angleDegree;

    addItem(pixmapItem);

    m_implantItems.append(pixmapItem);
    qDebug() << m_implantItems.size();

    qDebug() << "addImplantItem의 showTeeth" << showTeeth;
}

/* EUN JI */
//-------------------------------------------------------------------------------------------------
void Scene::addLineItem(QPointF stPos, QPointF edPos)
{
    QLineF line(stPos,edPos);
    QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
    lineItem->setPen(QPen(m_currentColor, paintSize));

    lineItem->setFlags(QGraphicsItem::ItemIsSelectable);

    addItem(lineItem);

    m_allitems.append(lineItem);
}

void Scene::addRectItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable);
    rectItem->setPen(QPen(rectItem->isSelected()?Qt::red:m_currentColor, rectItem->isSelected()?1:paintSize));

    addItem(rectItem);

    m_allitems.append(rectItem);
}

void Scene::addEllipseItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(rect);
    ellipseItem->setFlags(QGraphicsItem::ItemIsSelectable);
    ellipseItem->setPen(QPen(ellipseItem->isSelected()?Qt::red:m_currentColor, ellipseItem->isSelected()?1:paintSize));

    addItem(ellipseItem);

    m_allitems.append(ellipseItem);
}


void Scene::addTriangleItem(QPointF stPos,QPointF mdPos, QPointF edPos)
{
    QVector<QPoint> v;
    v<<QPoint(stPos.x(),stPos.y())<<QPoint(mdPos.x(),mdPos.y())<<QPoint(edPos.x(),edPos.y());

    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(QPolygonF(v));
    polygonItem->setFlags(QGraphicsItem::ItemIsSelectable);
    polygonItem->setPen(QPen(polygonItem->isSelected()?Qt::red:m_currentColor, polygonItem->isSelected()?1:paintSize));

    addItem(polygonItem);

    m_allitems.append(polygonItem);
}

void Scene::addArrowItem(QPointF stPos, QPointF edPos)
{
    QVector<QPoint> v;

    QPointF midPos;
    midPos = edPos-stPos;

    v<<QPoint(stPos.x()+(midPos.x()/2),stPos.y())<<QPoint(stPos.x(),stPos.y()+(midPos.y()/3))
    <<QPoint(stPos.x()+(midPos.x()*2/5),stPos.y()+(midPos.y()/3))<<QPoint(stPos.x()+(midPos.x()*2/5),edPos.y())
    <<QPoint(stPos.x()+(midPos.x()*3/5),edPos.y()) <<QPoint(stPos.x()+(midPos.x()*3/5),stPos.y()+(midPos.y()/3))
    <<QPoint(edPos.x(),stPos.y()+(midPos.y()/3));

    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(QPolygonF(v));
    polygonItem->setFlags(QGraphicsItem::ItemIsSelectable);
    polygonItem->setPen(QPen(polygonItem->isSelected()?Qt::red:m_currentColor, polygonItem->isSelected()?1:paintSize));
    polygonItem->setBrush(m_currentColor);

    addItem(polygonItem);

    m_allitems.append(polygonItem);
    qDebug()<< m_allitems.size();
}

void Scene::addTextItem(QPointF stPos)
{
    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);

    textItem->setFlags(QGraphicsItem::ItemIsSelectable);
    textItem->setScale(paintSize);
    textItem->setDefaultTextColor(m_currentColor);

    textItem->setPos(stPos);

    addItem(textItem);

    m_allitems.append(textItem);
}

void Scene::keyPressEvent(QKeyEvent *event) // delete Key 입력 시 아이템 삭제
{
    if(event->key() == Qt::Key_Delete) {
        if(selectedItems().count()>0) {

            foreach(auto item, selectedItems())
                removeItem(item);
            update();
        }
    }
}

void Scene::deleteItems()
{
    if(selectedItems().count()>0) {

        foreach(auto item, selectedItems())
            removeItem(item);
        update();

    }
}

void Scene::rotate(){
    foreach(auto item, selectedItems()){
        QPointF offset = item->boundingRect().center();

        QTransform transform;
        transform.translate(offset.x(),offset.y());
        transform.rotate(rotateAngle);
        transform.translate(-offset.x(),-offset.y());
        item->setTransform(transform);
    }
    update();
}


void Scene::setPaintSize(qreal size){

    paintSize=size;
}


void Scene::setRotation(qreal angle){

    rotateAngle=angle;
}

void Scene::setText(QString string){

    text = string;
}

//------------------------------------------------------------------------------------------------------------------------------- EUN JI


void Scene::clearItems()
{
/* EUN JI */
//------------------------------------------------------
    /* 모든 도형 지우기 */
    foreach(auto items, m_allitems)
        removeItem(items);

    /* 브러쉬 지우기 */
    foreach(auto items, m_pathList)
        removeItem(items);
//------------------------------------------------------
}

void Scene::implantClearItems()
{
    foreach(auto items, m_implantItems)
        removeItem(items);

    foreach(auto pointitem, m_point)
        removeItem(pointitem);

    m_imCount = 1;              // 삭제할 때 좌표(순서 점) 초기화
}

/* JAE HY */
//---------------------------------------------------------
void Scene::reFirstOrtho(int _x, int _y)
{
    lengthFstX = _x;
    lengthFstY = _y;
}

void Scene::reLastOrtho(int _x, int _y)
{
    lengthLastX = _x;
    lengthLastY = _y;
}

void Scene::reFirstAnglePoint(int _x, int _y)
{
    fstPosX = _x, fstPosY = _y;
}

void Scene::reSecondAnglePoint(int _x, int _y)
{
    sedPosX = _x, sedPosY = _y;
}

void Scene::reThirdAnglePoint(int _x, int _y)
{
    trdPosX = _x, trdPosY = _y;
}

/*길이 측정 아이템 선택시 아이템 상태 설정*/
void Scene::addLengthItem(QPointF stPos, QPointF edPos)
{
    QLineF line(stPos,edPos);                                           // 설정된 시작 지점과 끝지점에 line 변수 선언
    QGraphicsLineItem *lengthItem = new QGraphicsLineItem(line);        // QGraphicsLineItem 할당
    lengthItem->setPen(QPen(Qt::green, paintSize));         // 초기 길이 측정 색상은 초록색으로 설정
    addItem(lengthItem);                                    // Scene 상의 아이템 추가

    m_allitems.append(lengthItem);          // 전체 아이템 리스트에 추가
}

/*각도 측정 아이템 선택시 아이템 상태 설정*/
void Scene::addAngleItem(QPointF stPos,QPointF mdPos, QPointF edPos)
{
    QPolygonF polygon1;
    polygon1 << QPointF(stPos) << QPointF(mdPos);         // 첫 번째 선분(polygon) 좌표 할당
    QPolygonF polygon2;
    polygon2 << QPointF(mdPos) << QPointF(edPos);         // 두 번째 선분(polygon) 좌표 할당

    /*할당된 선분들을 추가*/
    QPainterPath polyLineItem;
    polyLineItem.addPolygon(polygon1);
    polyLineItem.addPolygon(polygon2);

    QGraphicsPathItem *polyLine = new QGraphicsPathItem(polyLineItem); // 폴리곤 아이템 객체 할당
    polyLine->setPen(QPen(Qt::blue, paintSize));         // 초기 각도 측정 색상은 파란색으로 설정

    addItem(polyLine);                      // 각도 측정 추가
    m_allitems.append(polyLine);            // 전체 아이템 리스트에 추가
}
//----------------------------------------------------------------------------


/* Top Left Teeth */
void Scene::slot_11(QPixmap* teeth11)
{
    qDebug() << "치아 11번 슬롯이 동작합니다.";

    showTeeth = teeth11;        // 11번 임플란트 이미지
}

void Scene::slot_12(QPixmap* teeth12)
{
    showTeeth = teeth12;        // 12번 임플란트 이미지
}

void Scene::slot_13(QPixmap* teeth13)
{
    showTeeth = teeth13;        // 13번 임플란트 이미지
}

void Scene::slot_14(QPixmap* teeth14)
{
    showTeeth = teeth14;        // 14번 임플란트 이미지
}

void Scene::slot_15(QPixmap* teeth15)
{
    showTeeth = teeth15;        // 15번 임플란트 이미지
}

void Scene::slot_16(QPixmap* teeth16)
{
    showTeeth = teeth16;        // 16번 임플란트 이미지
}

void Scene::slot_17(QPixmap* teeth17)
{
    showTeeth = teeth17;        // 17번 임플란트 이미지
}


/* Top Right Teeth */
void Scene::slot_21(QPixmap* teeth21)
{
    showTeeth = teeth21;        // 21번 임플란트 이미지
}
void Scene::slot_22(QPixmap* teeth22)
{
    showTeeth = teeth22;        // 22번 임플란트 이미지
}
void Scene::slot_23(QPixmap* teeth23)
{
    showTeeth = teeth23;        // 23번 임플란트 이미지
}
void Scene::slot_24(QPixmap* teeth24)
{
    showTeeth = teeth24;        // 24번 임플란트 이미지
}
void Scene::slot_25(QPixmap* teeth25)
{
    showTeeth = teeth25;        // 25번 임플란트 이미지
}
void Scene::slot_26(QPixmap* teeth26)
{
    showTeeth = teeth26;        // 26번 임플란트 이미지
}
void Scene::slot_27(QPixmap* teeth27)
{
    showTeeth = teeth27;        // 27번 임플란트 이미지
}


/* Top Right Teeth */
void Scene::slot_31(QPixmap* teeth31)
{
    showTeeth = teeth31;        // 31번 임플란트 이미지
}
void Scene::slot_32(QPixmap* teeth32)
{
    showTeeth = teeth32;        // 32번 임플란트 이미지
}
void Scene::slot_33(QPixmap* teeth33)
{
    showTeeth = teeth33;        // 33번 임플란트 이미지
}
void Scene::slot_34(QPixmap* teeth34)
{
    showTeeth = teeth34;        // 34번 임플란트 이미지
}
void Scene::slot_35(QPixmap* teeth35)
{
    showTeeth = teeth35;        // 35번 임플란트 이미지
}
void Scene::slot_36(QPixmap* teeth36)
{
    showTeeth = teeth36;        // 36번 임플란트 이미지
}
void Scene::slot_37(QPixmap* teeth37)
{
    showTeeth = teeth37;        // 37번 임플란트 이미지
}


/* Bottom Left Teeth */
void Scene::slot_41(QPixmap* teeth41)
{
    showTeeth = teeth41;        // 41번 임플란트 이미지
}
void Scene::slot_42(QPixmap* teeth42)
{
    showTeeth = teeth42;        // 42번 임플란트 이미지
}
void Scene::slot_43(QPixmap* teeth43)
{
    showTeeth = teeth43;        // 43번 임플란트 이미지
}
void Scene::slot_44(QPixmap* teeth44)
{
    showTeeth = teeth44;        // 44번 임플란트 이미지
}
void Scene::slot_45(QPixmap* teeth45)
{
    showTeeth = teeth45;        // 45번 임플란트 이미지
}
void Scene::slot_46(QPixmap* teeth46)
{
    showTeeth = teeth46;        // 46번 임플란트 이미지
}
void Scene::slot_47(QPixmap* teeth47)
{
    showTeeth = teeth47;        // 47번 임플란트 이미지
}





