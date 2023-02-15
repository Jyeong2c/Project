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

/* 마우스를 클릭했을 때 실행하는 함수 override */
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
        /* 임플란트를 선택한 경우 */
        else if(m_currentShape == Implant) {
            if(m_imCount == 1) {    // m_imCount = 1인 경우
                m_prevPos = event->scenePos();  // m_prevPos : 첫 번째 좌표값 (Top Left)
                m_imCount++;

                /* 클릭했을 때 좌표 점 찍기 (빨간점) */
                QBrush brush(QColor(Qt::red), Qt::SolidPattern);
                QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
                item1->setBrush(brush);
                item1->setRect(0, 0, ItemWidth, ItemWidth);
                item1->setPos(m_prevPos);
                addItem(item1);
                m_point.append(item1);
            }
            else if(m_imCount == 2) {   // m_imCount = 2인 경우
                m_latePos = event->scenePos();  // m_latePos : 두 번째 좌표값 (Bottom Left)
                m_imCount++;

                /* 클릭했을 때 좌표 점 찍기 (파란점) */
                QBrush brush(QColor(Qt::blue), Qt::SolidPattern);
                QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem();
                item2->setBrush(brush);
                item2->setRect(0, 0, ItemWidth, ItemWidth);
                item2->setPos(m_latePos);
                addItem(item2);
                m_point.append(item2);
            }
            else if(m_imCount == 3) {   // m_imCount = 3인 경우
                m_sPos = event->scenePos(); // m_sPos : 세 번째 좌표값 (Right의 임의의 한 점의 좌표값 = width의 역할)
                addImplantItem(m_prevPos, m_latePos, m_sPos);   // 임플란트를 식립하는 함수 (첫번째 좌표, 두번째 좌표, 세번째 좌표)

                /* 클릭했을 때 좌표 점 찍기 (노란점) */
                QBrush brush(QColor(Qt::yellow), Qt::SolidPattern);
                QGraphicsEllipseItem* item3 = new QGraphicsEllipseItem();
                item3->setBrush(brush);
                item3->setRect(0, 0, ItemWidth, ItemWidth);
                item3->setPos(m_sPos);
                addItem(item3);
                m_point.append(item3);

                foreach(auto pointitem, m_point)
                    removeItem(pointitem);

                m_imCount = 1;  // m_imCount의 값을 다시 1로 초기화
                m_currentShape = Cursor;    // 임플란트를 1번 출력한 후 현재 상태를 다시 커서로 변경
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
                if(m_triClicked == 1) {                                                     // 클릭 카운트가 1인 경우
                    emit firstAnglePoint(event->scenePos().x(), event->scenePos().y());     // 1번째 x,y좌표 전송
                    m_prevPos = event->scenePos();                                          // 현재 클릭한 위치의 좌표 변수 저장
                    m_triClicked++;                                                         // triClick 카운트 1 증가 triClick = 2

                } else if(m_triClicked == 2) {                                              // 클릭 카운트가 2인 경우
                    emit secondAnglePoint(event->scenePos().x(), event->scenePos().y());    // 2번째 x,y좌표 전송
                    m_midPos = event->scenePos();                                           // 현재 클릭한 위치의 좌표 변수 저장
                    m_triClicked++;                                                         // triClick 카운트 1 증가 triClick = 3

                } else if(m_triClicked == 3) {                                              // 클릭 카운트가 3인 경우
                    emit thirdAnglePoint(event->scenePos().x(), event->scenePos().y());     // 3번째 x,y좌표 전송
                    m_latePos = event->scenePos();                                          // 현재 클릭한 위치의 좌표 변수 저장


                    /*3점의 좌표를 지정하면 해당 좌표의 1,2 2,3 1,3 점끼리 선분을 생성*/
                    int a = qSqrt(qPow((sedPosX - fstPosX), 2) + qPow((sedPosY - fstPosY), 2)); // 1, 2번째 좌표 선분
                    int b = qSqrt(qPow((trdPosX - sedPosX), 2) + qPow((trdPosY - sedPosY), 2)); // 2, 3번째 좌표 선분
                    int c = qSqrt(qPow((fstPosX - trdPosX), 2) + qPow((fstPosY - trdPosY), 2)); // 3, 1번째 좌표 선분

                    /*세 점의 좌표를 찍으면 2번째 사이각의 각도를 구하는 공식*/
                    /*그러나 현 각도는 180도 내에서만 각도를 측정할 수 밖에 없음.*/
                    auto ang = qAcos((qPow(a, 2) + qPow(b, 2) - qPow(c, 2)) / (2 * a * b)) * 180 / 3.141592;    // c 선분 대입각 공식
                    emit sendAngleMeasure(round(ang));                  // 반올림된 각도 전송

                    addAngleItem(m_prevPos, m_midPos, m_latePos);       // 1, 2, 3 번째 좌표의 각도 아이템 추가
                    m_triClicked = 1;                                   // triangle 카운터 1로 초기화
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

/* 마우스를 누르고 움직였을 때 실행하는 함수 override */
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

/* 마우스를 눌렀다가 뗐을 때 실행하는 함수 override */
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_currentItem != nullptr)
        delete m_currentItem;

    switch(m_currentShape){

    case Cursor:
        break;

    case Implant:   // 현재 상태가 임플란트인 경우
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
            emit sendLastOrtho(event->scenePos().x(), event->scenePos().y());               // 마지막으로 찍은 좌표의 위치 전달
            int sceneWidth = sceneRect().topRight().x() - sceneRect().topLeft().x();        // 해당 scene의 width
            int sceneHeight = sceneRect().bottomRight().y() - sceneRect().topRight().y();   // 해당 scene의 height

            /*이미지 크기를 재조정 하기 위한 변수 선언*/
            double avgWidth = (double)imageWidth / sceneWidth;                              // width 재조정
            double avgHeight = (double)imageHeight / sceneHeight;                           // height 재조정

            double leng = qSqrt(qPow((lengthLastX - lengthFstX) * avgWidth, 2) +
                                qPow((lengthLastY - lengthFstY) * avgHeight, 2));           // 길이 측정 결과값 공식

            sendLengthMeasure(round(leng * imagePixel * 1000.0) / 100.0);                   // 결과 값의 픽셀당 길이 값을 곱한후 mm로 조정
            pointCount = 0;                     // 길이 카운터 초기화
        }
        break;
    }
    case Angle:
        break;
//---------------------------------------------------------------------------
    }
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

/* 임플란트를 출력하는 함수 */
void Scene::addImplantItem(QPointF stPos, QPointF edPos, QPointF mPos)
{
    /* 두 점(첫번째 좌표와 두번째 좌표)의 중간 좌표 값 */
    int xPos = (edPos.x() + stPos.x())/2;   // (첫번째 좌표의 x값 + 두번째 좌표의 x값)/2 = 두 점의 중간 x값
    int yPos = (edPos.y() + stPos.y())/2;   // (첫번째 좌표의 y값 + 두번째 좌표의 y값)/2 = 두 점의 중간 y값
    int line = sqrt(pow(xPos - mPos.x(), 2) + pow(yPos - mPos.y(), 2)); // 루트(제곱(중간 좌표의 x값 - 세번째 좌표의 x값) + 제곱(중간 좌표의 y값 - 세번째 좌표의 y값)) = 임플란트의 폭(width)

    QVector2D dot1(stPos);
    QVector2D dot2(edPos);

    float distance1 = dot1.distanceToPoint(dot2);   // 첫번째 좌표와 두번째 좌표의 거리(= height)

    QSizeF size(line, distance1); // 임플란트 크기(가로(width), 세로(height))
    QRectF rect(stPos, size);   // 임플란트 영역(TopLeft, BottomRight)

    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(showTeeth->scaled(rect.normalized().size().toSize(), Qt::IgnoreAspectRatio)); // showTeeth(임플란트 icon) 크기를 좌표의 크기만큼 설정
    pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable);
    pixmapItem->setPos(stPos);

    double angleRadian = atan2((stPos.x()-edPos.x()), (stPos.y()-edPos.y()));   // actan((첫번째 좌표 x값 - 두번째 좌표 x값), (첫번째 좌표 y값 - 두번째 좌표 y값))
    double angleDegree = floor(qRadiansToDegrees(angleRadian)); // 라디안을 각도로 변환
    pixmapItem->setRotation(180 - angleDegree);
    addItem(pixmapItem);
    m_implantItems.append(pixmapItem);
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

void Scene::clearItems()
{

    /* 모든 도형 지우기 */
    foreach(auto items, m_allitems)
        removeItem(items);

    /* 브러쉬 지우기 */
    foreach(auto items, m_pathList)
        removeItem(items);

}
//------------------------------------------------------------------------------------------------------------------------------- EUN JI

/* 임플린트 초기화 */
void Scene::implantClearItems()
{
    foreach(auto items, m_implantItems)
        removeItem(items);  // 임플란트 제거
    m_imCount = 1;
}


/* JAE HY */
//---------------------------------------------------------
/*길이 측정 시 첫번째 좌표 받는 슬롯*/
void Scene::reFirstOrtho(int _x, int _y)
{
    lengthFstX = _x;
    lengthFstY = _y;
}

/*길이 측정 시 마지막 좌표를 받는 슬롯*/
void Scene::reLastOrtho(int _x, int _y)
{
    lengthLastX = _x;
    lengthLastY = _y;
}

/*각도 측정 시 첫번째 좌표를 받는 슬롯*/
void Scene::reFirstAnglePoint(int _x, int _y)
{
    fstPosX = _x, fstPosY = _y;
}

/*각도 측정 시 두번째 좌표를 받는 슬롯*/
void Scene::reSecondAnglePoint(int _x, int _y)
{
    sedPosX = _x, sedPosY = _y;
}

/*각도 측정 시 세번째 좌표를 받는 슬롯*/
void Scene::reThirdAnglePoint(int _x, int _y)
{
    trdPosX = _x, trdPosY = _y;
}

/*이미지의 width, height, pixel 정보를 얻고 할당된 변수에 저장*/
void Scene::reImageWHP(int _width, int _height, double _pixels)
{
    /*한번 클릭했던 Scene의 이미지 정보만 받음*/
    imageWidth = _width, imageHeight = _height, imagePixel = _pixels;
}

/*길이 측정 아이템 선택시 아이템 상태 설정*/
void Scene::addLengthItem(QPointF stPos, QPointF edPos)
{
    QLineF line(stPos,edPos);                                           // 설정된 시작 지점과 끝지점에 line 변수 선언
    QGraphicsLineItem *lengthItem = new QGraphicsLineItem(line);        // QGraphicsLineItem을 line으로 초기화
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
    polyLineItem.addPolygon(polygon1);              // polygon1 변수 addPolygon
    polyLineItem.addPolygon(polygon2);              // polygon2 변수 addPolygon

    QGraphicsPathItem *polyLine = new QGraphicsPathItem(polyLineItem); // QGraphicsPathItem을 polyLineItem으로 초기화
    polyLine->setPen(QPen(Qt::blue, paintSize));         // 초기 각도 측정 색상은 파란색으로 설정

    addItem(polyLine);                      // 각도 측정 추가
    m_allitems.append(polyLine);            // 전체 아이템 리스트에 추가
}
//----------------------------------------------------------------------------


/* Top Left Teeth */
void Scene::slot_11(QPixmap* teeth11)
{
    showTeeth = teeth11;    // 11번 임플란트 이미지
}

void Scene::slot_12(QPixmap* teeth12)
{
    showTeeth = teeth12;    // 12번 임플란트 이미지
}

void Scene::slot_13(QPixmap* teeth13)
{
    showTeeth = teeth13;    // 13번 임플란트 이미지
}

void Scene::slot_14(QPixmap* teeth14)
{
    showTeeth = teeth14;    // 14번 임플란트 이미지
}

void Scene::slot_15(QPixmap* teeth15)
{
    showTeeth = teeth15;    // 15번 임플란트 이미지
}

void Scene::slot_16(QPixmap* teeth16)
{
    showTeeth = teeth16;    // 16번 임플란트 이미지
}

void Scene::slot_17(QPixmap* teeth17)
{
    showTeeth = teeth17;    // 17번 임플란트 이미지
}

/* Top Right Teeth */
void Scene::slot_21(QPixmap* teeth21)
{
    showTeeth = teeth21;    // 21번 임플란트 이미지
}
void Scene::slot_22(QPixmap* teeth22)
{
    showTeeth = teeth22;    // 22번 임플란트 이미지
}
void Scene::slot_23(QPixmap* teeth23)
{
    showTeeth = teeth23;    // 23번 임플란트 이미지
}
void Scene::slot_24(QPixmap* teeth24)
{
    showTeeth = teeth24;    // 24번 임플란트 이미지
}
void Scene::slot_25(QPixmap* teeth25)
{
    showTeeth = teeth25;    // 25번 임플란트 이미지
}
void Scene::slot_26(QPixmap* teeth26)
{
    showTeeth = teeth26;    // 26번 임플란트 이미지
}
void Scene::slot_27(QPixmap* teeth27)
{
    showTeeth = teeth27;    // 27번 임플란트 이미지
}

/* Top Right Teeth */
void Scene::slot_31(QPixmap* teeth31)
{
    showTeeth = teeth31;    // 31번 임플란트 이미지
}
void Scene::slot_32(QPixmap* teeth32)
{
    showTeeth = teeth32;    // 32번 임플란트 이미지
}
void Scene::slot_33(QPixmap* teeth33)
{
    showTeeth = teeth33;    // 33번 임플란트 이미지
}
void Scene::slot_34(QPixmap* teeth34)
{
    showTeeth = teeth34;    // 34번 임플란트 이미지
}
void Scene::slot_35(QPixmap* teeth35)
{
    showTeeth = teeth35;    // 35번 임플란트 이미지
}
void Scene::slot_36(QPixmap* teeth36)
{
    showTeeth = teeth36;    // 36번 임플란트 이미지
}
void Scene::slot_37(QPixmap* teeth37)
{
    showTeeth = teeth37;    // 37번 임플란트 이미지
}

/* Bottom Left Teeth */
void Scene::slot_41(QPixmap* teeth41)
{
    showTeeth = teeth41;    // 41번 임플란트 이미지
}
void Scene::slot_42(QPixmap* teeth42)
{
    showTeeth = teeth42;    // 42번 임플란트 이미지
}
void Scene::slot_43(QPixmap* teeth43)
{
    showTeeth = teeth43;    // 43번 임플란트 이미지
}
void Scene::slot_44(QPixmap* teeth44)
{
    showTeeth = teeth44;    // 44번 임플란트 이미지
}
void Scene::slot_45(QPixmap* teeth45)
{
    showTeeth = teeth45;    // 45번 임플란트 이미지
}
void Scene::slot_46(QPixmap* teeth46)
{
    showTeeth = teeth46;    // 46번 임플란트 이미지
}
void Scene::slot_47(QPixmap* teeth47)
{
    showTeeth = teeth47;    // 47번 임플란트 이미지
}


