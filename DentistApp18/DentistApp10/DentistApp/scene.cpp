#include "scene.h"

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QKeyEvent>
#include <QtMath>

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}, drawing(false), m_currentColor(Qt::red)
{

    m_currentItem = nullptr;

    /*길이 측정을 위한 두점을 Scene클래스 내에서 처리하기 위한 커넥트*/
    connect(this, &Scene::sendFirstOrtho, &Scene::reFirstOrtho);
    connect(this, &Scene::sendLastOrtho, &Scene::reLastOrtho);

    /*connect 모음 (각도를 측정하기 위한 세점의 좌표를 보내는 신호와 슬롯을 연결*/
    connect(this, &Scene::firstAnglePoint, &Scene::reFirstAnglePoint);
    connect(this, &Scene::secondAnglePoint, &Scene::reSecondAnglePoint);
    connect(this, &Scene::thirdAnglePoint, &Scene::reThirdAnglePoint);
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)    // 마우스 클릭 시
{

    QPen pen(m_currentColor, 3);
    //if(items(event->scenePos()).isEmpty()){

    /*m_currentShape => 기능을 구분짓는 변수*/
    if(m_currentShape == Path){
        drawing = true;

        QPainterPath path;
        QGraphicsPathItem* item = addPath(path);
        item->setPen(pen);
        item->setBrush(Qt::transparent);

        path = item->path();
        path.moveTo(event->scenePos());
        //path.lineTo(event->scenePos());
        item->setPath(path);

        pathList.append(item);
    } else if(m_currentShape == Length){
        m_startPos = event->scenePos();
        emit sendFirstOrtho(event->scenePos().x(), event->scenePos().y());
        qDebug() << "Scene(mousePress) : " << event->scenePos().x() << ", " << event->scenePos().y();
    } else if(m_currentShape == Angle) {
        switch(pointCount)
        {
        case 0:
            emit firstAnglePoint(event->scenePos().x(), event->scenePos().y());
            qDebug() << "firstAnglePoint ( " << fstPosX << ", " << fstPosY << ")";
            /*첫번째 좌표를 누를 시 해당 지점 부터 좌표 측정*/
            m_fstAnglePos = QPointF(fstPosX, fstPosY);
            break;
        case 1:
            m_sedAnglePos = QPointF(sedPosX, sedPosY);
            break;
        }
    } else{
        /*enum Path 이외의 상수들은 m_startPos변수로 시작점을 잡음*/
        m_startPos = event->scenePos();
        qDebug() << "first X Pos : " << event->scenePos().x() << ", first Y Pos : " << event->scenePos().y();
    }
    QGraphicsScene::mousePressEvent(event);
}
/*길이 측정 좌표를 설정받는 set함수*/
void Scene::setLengFirstXY(int &x, int& y)
{
    lengthFstX = x;
    lenghtFstY = y; //길이 측정 첫번째 x, y 좌표
}

void Scene::setLengLastXY(int &x, int& y)
{
    lengthLastX = x;
    lengthLastY = y;
}

/*각도 좌표를 설정받는 set함수 -> 3개의 함수로 줄여보기*/
void Scene::setAngFirstXY(int &x, int& y)
{
    fstPosX = x; fstPosY = y;
}
void Scene::setAngSecondXY(int &x, int& y)
{
    sedPosX = x; sedPosY = y;
}
void Scene::setAngThirdXY(int &x, int &y)
{
    trdPosX = x; trdPosY = y;
}

//////////////////////////////////////////////////////

/*Scene 클래스 내에서 길이 좌표 시그널을 받는 슬롯*/
void Scene::reFirstOrtho(int _x, int _y)
{
    setLengFirstXY(_x, _y);
}

void Scene::reLastOrtho(int _x, int _y)
{
    setLengLastXY(_x, _y);
}

/*Scene 클래스 내에서 각도 좌표 시그널을 받는 슬롯*/
void Scene::reFirstAnglePoint(int _x, int _y)
{
    setAngFirstXY(_x, _y);
}

void Scene::reSecondAnglePoint(int _x, int _y)
{
    setAngSecondXY(_x, _y);
}

void Scene::reThirdAnglePoint(int _x, int _y)
{
    setAngThirdXY(_x, _y);
}
/////////////////////////////////////////////////////

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(m_currentShape == Path){
        if(drawing){
            QGraphicsPathItem* item = pathList.last();

            if(item){

                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
        }
    } else if(m_currentShape == Line){
        if(m_currentItem != nullptr)
            delete m_currentItem;

        /* 드래그 시 Line을 graphicsview에 보여줌 */

        QLineF line(m_startPos, event->scenePos());
        QGraphicsLineItem *item = new QGraphicsLineItem(line);
        item->setPen(QPen(QColor(Qt::blue),1));
        addItem(item);
        m_currentItem = item;

    } else if(m_currentShape == Length){
        /*길이 측정 기능을 마우스 이동중에 QGraphicsLineItem으로 안티에일리어싱 하면서 직선을 그림*/
        if(m_currentItem != nullptr)
            delete m_currentItem;

        QLineF lengthLine(m_startPos, event->scenePos());
        itemToDraw = new QGraphicsLineItem(lengthLine);
        itemToDraw->setPen(QPen(QColor(Qt::black), 1));
        addItem(itemToDraw);
        m_currentItem = itemToDraw;

    } else if(m_currentShape == Angle){
        /*각도 측정 기능을 마우스 이동중에 QGraphicsLineItem으로 안티에일리어싱 하면서 직선을 그림*/
        if(m_currentItem != nullptr)
            delete m_currentItem;
        if(pointCount == 0){
            QLineF AngleLine(m_fstAnglePos, event->scenePos());
            itemToDraw = new QGraphicsLineItem(AngleLine);
            itemToDraw->setPen(QPen(QColor(Qt::black), 1));
            addItem(itemToDraw);
            m_currentItem = itemToDraw;
        }
        if(pointCount == 1){
            QLineF AngleLine(m_sedAnglePos, event->scenePos());
            itemToDraw = new QGraphicsLineItem(AngleLine);
            itemToDraw->setPen(QPen(QColor(Qt::black), 1));
            addItem(itemToDraw);
            m_currentItem = itemToDraw;
        }
    }

    else{

        if(m_currentItem != nullptr)
            delete m_currentItem;

        /* 드래그 시 사각형을 graphicsview에 보여줌 */

        QRectF rect(m_startPos, event->scenePos());
        QGraphicsRectItem *item = new QGraphicsRectItem(rect);
        item->setPen(QPen(QColor(Qt::blue),1));
        addItem(item);
        m_currentItem = item;

    }

    //m_startPos = event->scenePos();
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //        if(m_currentItem != nullptr)
    //        delete m_currentItem;
    switch(m_currentShape){
    case Line:
        addLineItem(m_startPos, event->scenePos());
        break;

    case Rect:
        addRectItem(m_startPos, event->scenePos());
        break;

    case Ellipse:
        addEllipseItem(m_startPos, event->scenePos());
        break;

    case Path:
        if(drawing){
            QGraphicsPathItem* item = pathList.last();

            if(item){
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
                item->setFlag(QGraphicsItem::ItemIsMovable,true);
                item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            }
            drawing = false;
        }
        break;

    case Image:
        addImageItem(m_startPos, event->scenePos());
        break;


    case Length:
    {
        /*길이 아이템이 선택되는 모드로 설정*/
        addLengthItem(m_startPos, event->scenePos());
        //qDebug() << "Scene(mouseRelease) : " << event->scenePos().x() << ", " << event->scenePos().y();
        int sceneWidth = sceneRect().topRight().x() - sceneRect().topLeft().x();
        int sceneHeight = sceneRect().bottomRight().y() - sceneRect().topRight().y();
        qDebug() << "sceneWidth : " << sceneWidth;
        qDebug() << "sceneHeight : " << sceneHeight;
        emit sendLastOrtho(event->scenePos().x(), event->scenePos().y());
        /*서버의 이미지 크기도 여기에서 수정되어야 함.*/
        double avgWidth = (double)400 / sceneWidth;
        double avgHeight = (double)200 / sceneHeight;

        double leng = qSqrt(qPow((lengthLastX - lengthFstX) * avgWidth, 2) +
                            qPow((lengthLastY - lenghtFstY) * avgHeight, 2));
        qDebug() << "Length : " <<  leng << "cm";
        /*길이 측정 첫번째 좌표와 결과값을 출력하는 슬롯 함수 구현부*/

        /*소수점 세자리 까지 반올림*/
        emit sendMeasureLength(leng /** imagePixel*/);
        break;
    }

    case Angle:
        switch(pointCount)
        {
        case 0:
            emit secondAnglePoint(event->scenePos().x(), event->scenePos().y());
            addAngleItem(m_fstAnglePos, event->scenePos());
            qDebug() << "secondAnglePoint ( " << sedPosX << ", " << sedPosY << ")";
            pointCount++;
            break;
        case 1:
            emit thirdAnglePoint(event->scenePos().x(), event->scenePos().y());
            addAngleItem(m_sedAnglePos, event->scenePos());
            qDebug() << "thridAnglePoint ( " << trdPosX << ", " << trdPosY << ")";


            /*모든 좌표 설정을 마친 후 각도를 계산 이후 포인터 카운트 초기화*/
            if(pointCount == 1){
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
                /*측정한 결과값을 메인윈도우로 넘김*/
                emit sendMeasureAngle(ang);
                pointCount = 0;
            }
            break;
        }

        break;
    }
    m_currentItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}


Scene::Shape Scene::getCurrentShape() const
{
    return m_currentShape;
}

void Scene::setCurrentShape(const Shape &value)
{
    m_currentShape = value;
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    /*길이, 각도 측정을 하였을 때 5, 6으로 표시되는 것을 확인*/
    qDebug() << "value : " << m_currentShape;
}

QColor Scene::getCurrentColor() const
{
    return m_currentColor;
}

void Scene::setCurrentColor(const QColor& color)
{

    m_currentColor = color;
}

void Scene::addLineItem(QPointF stPos, QPointF edPos)
{
    QLineF line(stPos,edPos);
    QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
    lineItem->setPen(QPen(m_currentColor, 3));

    lineItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem *boundRectItem = new QGraphicsRectItem();
    boundRectItem->setRect(lineItem->boundingRect().adjusted(-2,-2,2,2));
    boundRectItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    boundRectItem->setPen(QPen(boundRectItem->isSelected()?Qt::red:Qt::transparent,1));
    lineItem->setParentItem(boundRectItem);

    //addItem(lineItem);
    addItem(boundRectItem);
}

/*길이 측정 기능 출력후 Scene상 Pen은 검정색, focusable(고정)로 플래깅*/
void Scene::addLengthItem(QPointF stPos, QPointF edPos)
{
    QLineF line(stPos, edPos);
    //QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
    //lineItem->setPen(QPen(Qt::black, 2));
    itemToDraw->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    itemToDraw->setPen(QPen(Qt::green, 3));

    QGraphicsRectItem *boundRectItem = new QGraphicsRectItem();
    boundRectItem->setRect(itemToDraw->boundingRect().adjusted(-2,-2,2,2));
    boundRectItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);

    boundRectItem->setPen(QPen(boundRectItem->isSelected()?Qt::red:Qt::transparent,1));
    itemToDraw->setParentItem(boundRectItem);

    //addItem(lineItem);
    addItem(boundRectItem);
}

/*각도 측정 기능 출력후 Scene상 Pen은 검정색, focusable(고정)로 플래깅*/
void Scene::addAngleItem(QPointF stPos, QPointF edPos)
{
    QLineF line(stPos, edPos);
    //QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
    //lineItem->setPen(QPen(Qt::black, 2));
    itemToDraw->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    itemToDraw->setPen(QPen(Qt::blue, 3));

    QGraphicsRectItem *boundRectItem = new QGraphicsRectItem();
    boundRectItem->setRect(itemToDraw->boundingRect().adjusted(-2,-2,2,2));
    boundRectItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);

    boundRectItem->setPen(QPen(boundRectItem->isSelected()?Qt::red:Qt::transparent,1));
    itemToDraw->setParentItem(boundRectItem);

    //addItem(lineItem);
    addItem(boundRectItem);
}

void Scene::addRectItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
    rectItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    rectItem->setPen(QPen(rectItem->isSelected()?Qt::red:m_currentColor, rectItem->isSelected()?3:1));
    addItem(rectItem);
}

void Scene::addEllipseItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(rect);
    //ellipseItem->setBrush(m_currentColor);    // 채우기 색
    ellipseItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    ellipseItem->setPen(QPen(ellipseItem->isSelected()?Qt::red:m_currentColor, ellipseItem->isSelected()?3:1));
    addItem(ellipseItem);

}

void Scene::addImageItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QPixmap pixmap("Implant3.png");
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap.scaled(rect.size().toSize(), Qt::KeepAspectRatio));

    QGraphicsRectItem * boundRectItem = new QGraphicsRectItem();
    boundRectItem->setRect(pixmapItem->boundingRect().adjusted(-5,-5,5,5));
    boundRectItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    boundRectItem->setPen(QPen(boundRectItem->isSelected()?Qt::red:Qt::transparent, 1));

    pixmapItem->setParentItem(boundRectItem);
    //    pixmapItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    boundRectItem->setPos(stPos);
    // pixmapItem->setPos(stPos);
    addItem(boundRectItem);
    // addItem(pixmapItem);
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete){
        if(selectedItems().count()>0){

            foreach(auto item, selectedItems())
                removeItem(item);
            update();

        }
    }
}

/*4개의 Scene중 택 1을 하였을 때 해당되는 Scene에 픽셀 당 길이, 크기정보를 Scene마다 받음*/
void Scene::reImageInfo(double _pixel, int _width, int _height)
{
    imagePixel = _pixel; imageWidth = _width; imageHeight = _height;
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    qDebug() << "imagePixel : " << imagePixel;
    qDebug() << "imageWidth : " << imageWidth;
    qDebug() << "imageHeight : " << imageHeight;
}
