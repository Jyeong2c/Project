#ifndef SCENE_H
#define SCENE_H

//#include <QObject>
#include <QGraphicsScene>

//class QGraphicsPathItem;
class QPointF;
class QGraphicsLineItem;
class QGraphicsTextItem;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    /*툴바 액션을 구분하는 enum 클래스*/
    enum Shape {Line, Rect, Ellipse, Path, Image, Length, Angle };

    Shape getCurrentShape() const;
    void setCurrentShape(const Shape &value);

    QColor getCurrentColor() const;
    void setCurrentColor(const QColor& color);

    /*각도 좌표를 설정받는 set함수 -> 3개의 함수로 줄여보기*/
    void setAngFirstXY(int &x, int& y);
    void setAngSecondXY(int &x, int& y);
    void setAngThirdXY(int &x, int &y);

signals:
    /*길이 측정 관련 시그널*/
    void sendFirstOrtho(int _x, int _y);
    void sendLastOrtho(int _x, int _y);

    /*각도 측정 관련 시그널*/
    void firstAnglePoint(int _x, int _y);
    void secondAnglePoint(int _x, int _y);
    void thirdAnglePoint(int _x, int _y);

    /*메인 윈도우에 보낼 각도 측정 결과 시그널*/
    void sendMeasureAngle(double _angle);

public slots:
    void reFirstAnglePoint(int _x, int _y);
    void reSecondAnglePoint(int _x, int _y);
    void reThirdAnglePoint(int _x, int _y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void addLineItem(QPointF stPos, QPointF edPos);
    void addRectItem(QPointF stPos, QPointF edPos);
    void addEllipseItem(QPointF stPos, QPointF edPos);
    void addImageItem(QPointF stPos, QPointF edPos);

    /*길이 측정 아이템 출력후 moveable 지정*/
    void addLengthItem(QPointF stPos, QPointF edPos);

    /*각도 측정 아이템 출력후 moveable 지정*/
    void addAngleItem(QPointF stPos, QPointF edPos);

private:
    bool drawing;
    // List
    QList<QGraphicsPathItem*> pathList;
//PathItem
    QGraphicsPathItem* m_pathItem;
    QGraphicsItem* m_currentItem;

    QColor m_currentColor;
    Shape m_currentShape;

    /*그리는 모든 기능들은 m_startPos변수로 위치 지정*/
    QPointF m_startPos;

    /*각도 측정용 1번째, 2번째 좌표 설정*/
    QPointF m_fstAnglePos;
    QPointF m_sedAnglePos;

    QGraphicsLineItem *itemToDraw;

    /*텍스트로 길이를 텍스트로 표시하는 변수*/
    QGraphicsTextItem* textItem;

    int pointCount = 0;         //각도 측정 카운터

    int fstPosX, fstPosY;       //각도 측정 첫번째 x,y 좌표
    int sedPosX, sedPosY;       //각도 측정 두번째 x,y 좌표
    int trdPosX, trdPosY;       //각도 측정 세번째 x,y 좌표

public slots:
   // void updateScene();

};

#endif // SCENE_H
