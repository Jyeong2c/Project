#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;

class QGraphicsLineItem;
class QGraphicsSceneWheelEvent;
class QStyleOptionGraphicsItem;

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);

public slots:
    void brightUp();
    void brightnessLow();
    void inverstion();

    /*measure buttons*/
    void proTractor();
    void wheelEvent(QWheelEvent *ev) override;

    /*길이 측정을 위한 라인 마우스 이벤트*/
    void ruler();
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//    QRectF boundingRect() const;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
//    void paintEvent(QPaintEvent *e) override;

private:
    void gridLayout();
    QImage * image;
    QGraphicsView *grid1;
    QGraphicsScene* scene;

    /*길이, 각도를 측정하는 lineItem 변수 선정*/
    QGraphicsLineItem *lineItem;
    /*회전 각도 초기화*/
    int rotate = 0;            //시계방향으로 회전하는 각도 변수
    /*길이 측정 변수 설정*/
    bool m_nbMousePressed;

    QGraphicsLineItem *itemToDraw;
    QPointF origPoint;


signals:

};

#endif // LAYOUT_H
