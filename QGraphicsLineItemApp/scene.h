#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>

/*자식클래스(서브클래스)는 QGraphicsScene으로 서브 클래싱*/
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {NoMode, SelectObject, DrawLine};     //그리기 모드와 선택 모드를 구별하기 위해 열거형 모드
    Scene(QObject* parent = 0);
    void setMode(Mode mode);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);      //라인의 시작점을 지정 이 점을 QPointForigPos에 저장
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);       //QGraphicsLineItem의 인스턴스가 이미 존재하는지 확인
                                                                //그렇지 않은 경우 새 인스턴스를 호출 그 위치를 origPos로 설정(이 과정은 한번밖에 안함)
    /*mouseMoveEvent()에 대한 모든 후속 호출에서  QGraphicsLineItem을 정의하는 경계선을 업데이트한다.*/
    /*경계 선은 항상 (0,0)에서 시작하며, 끝점은 이벤트->scenePos()와 origPoint 사이의 거리에 따라 결정됩니다.
     * QGraphicsLineItem의 실제 위치는 'if 블록'에서 setPos(QPointF)로 표시되었습니다.*/
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Mode sceneMode;
    QPointF origPoint;                              //현재 그려진 항목의 시작 위치를 저장하는 QPointF 오리그포
    QGraphicsLineItem* itemToDraw;                  //현재 그려진 항목의 주소를 저장하는 변수
    void makeItemsControllable(bool areControllable);
    /*이벤트 핸들러를 재정의하는 것 외에도 장면 모드에 따라 항목의 선택 가능성과 이동성을 조정하는 도우미 기능 makeItemsControlable(boole)도 도입한다.*/

signals:
    void OrthoSend(int x, int y);
};

#endif // SCENE_H
