#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;
class QPushButton;
class QEvent;
class KeyFeaturesForm;
class QLabel;
class QGraphicsPixmapItem;
class QGraphicsSceneMouseEvent;

class View;
class Scene;


class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);

    /* 멤버 변수 */
    View *grid1;    // QGraphicsView
    View *grid2;    // QGraphicsView
    View *grid3;    // QGraphicsView
    View *grid4;    // QGraphicsView

    Scene *scene1;  // QGraphicsScene
    Scene *scene2;  // QGraphicsScene
    Scene *scene3;  // QGraphicsScene
    Scene *scene4;  // QGraphicsScene

    View *m_currentView;    // 현재 View를 담는 QGraphicsView
    Scene *m_currentScene;  // 현재 Scene을 담는 QGraphicsScene

    QPointF clickPoint; // 클릭한 좌표 위치

    bool g = false;
    bool s = false;
    bool _pan;
    int _panStartX, _panStartY;

    QMenu* menu;    // 메뉴

    QAction* zoomInAction;  // 확대 Action
    QAction* zoomOutAction; // 축소 Action
    QAction* rightRotateAction; // 90°회전 Action
    QAction* leftRotateAction;  // -90°회전 Action
    QAction* brushAction;   // 브러쉬 Action
    QAction* rectangleAction;   // 사각형 Action
    QAction* triangleAction;    // 삼각형 Action
    QAction* ellipseAction; // 원형 Action
    QAction* cursorAction;  // 커서 Action
    QAction* blendingAction;    // 블랜딩 Action
    QAction* lengthMeasurementAction;   // 길이 측정 Action
    QAction* angleMeasurementAction;    // 각도 측정 Action
    QAction* brightnessAction;  // 밝기 Action
    QAction* darknessAction;    // 어둡기 Action
    QAction* sharpenAction; // 선명화 Action
    QAction* contrastAction;    // 명암 Action
    QAction* inversionAction;   // 대비 Action
    QAction* horizontalFlipAction;  // 수평반전 Action
    QAction* verticalFlipAction;    // 수직반전 Action
    QAction* implantAction; // 임플란트 Action
    QAction* layoutImageClearAction; // 레이아웃 초기화 Action
    QAction* sourceSizeAction;  // 원본 크기 Action
    QAction* drawClearAction;   // Draw 초기화 Action
    QAction* implantClearAction;    // 임플란트 초기화 Action
    QAction* imageProcessingClearAction;    // 이미지 프로세싱 초기화 Action
    QAction* arrowAction;   // 화살표 Action
    QAction* lineAction;    // 선 Action


protected:
    void mousePressEvent(QMouseEvent *event) override;  // 마우스를 클릭했을 때 실행하는 함수 override
    void mouseDoubleClickEvent(QMouseEvent *event) override;    // 마우스를 더블 클릭했을 때 실행하는 함수 override
    void resizeEvent(QResizeEvent * event) override;    // Layout의 크기가 변경될 때 실행하는 함수 override


private:
    void gridLayout();  // 2 X 2 Layout
    void actionGroup(); // Key Features 기능에 필요한 Action들을 할당한 함수

    int slot_c1 = 0;
    int slot_c2 = 0;
    int slot_c3 = 0;
    int slot_c4 = 0;
    int slot_c5 = 0;
    int slot_c6 = 0;
    int slot_c7 = 0;
    int slot_c8 = 0;
    int slot_c9 = 0;
    int slot_c10 = 0;
    int slot_c11 = 0;
    int slot_c12 = 0;
    int slot_c13 = 0;
    int slot_c14 = 0;
    int slot_c15 = 0;
    int slot_c16 = 0;
    int slot_c17 = 0;
    int slot_c18 = 0;
    int slot_c19 = 0;
    int slot_c20 = 0;
    int slot_c21 = 0;
    int slot_c22 = 0;
    int slot_c23 = 0;
    int slot_c24 = 0;
    int slot_c25 = 0;
    int slot_c26 = 0;
    int slot_c27 = 0;

    int log_c1 = 0;
    int log_c2 = 0;
    int log_c3 = 0;
    int log_c4 = 0;
    int log_c5 = 0;
    int log_c6 = 0;
    int log_c7 = 0;
    int log_c8 = 0;
    int log_c9 = 0;
    int log_c10 = 0;
    int log_c11 = 0;
    int log_c12 = 0;
    int log_c13 = 0;
    int log_c14 = 0;
    int log_c15 = 0;
    int log_c16 = 0;
    int log_c17 = 0;
    int log_c18 = 0;
    int log_c19 = 0;
    int log_c20 = 0;
    int log_c21 = 0;
    int log_c22 = 0;
    int log_c23 = 0;
    int log_c24 = 0;
    int log_c25 = 0;
    int log_c26 = 0;
    int log_c27 = 0;


public slots:
    void slot_keyCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                       int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    void slot_logCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                       int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    void slot_save(bool save);


private slots:
    void showMenu(QPointF clickPoint);  // Action을 실행하는 슬롯


signals:
    void sig_widgetbyDClick();  // 마우스를 더블 클릭했을 때 발생하는 시그널
    void sig_point(QPointF);    // 마우스 가운데 버튼을 클릭했을 때 - 현재 좌표 위치를 시그널로 전달

/* EUN JI */
//------------------------------------------
    void sig_cursorChange();
};

#endif // LAYOUT_H
