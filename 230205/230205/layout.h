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

class Scene;


class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);

    QGraphicsView *grid1;
    QGraphicsView *grid2;
    QGraphicsView *grid3;
    QGraphicsView *grid4;

//    QGraphicsScene *scene1;
//    QGraphicsScene *scene2;
//    QGraphicsScene *scene3;
//    QGraphicsScene *scene4;

    Scene *scene1;
    Scene *scene2;
    Scene *scene3;
    Scene *scene4;

    QGraphicsView *grid;
//    QGraphicsScene *scene;

    Scene *scene;


    QPointF clickPoint;
    bool g = false;
    bool t = false;

    bool _pan;
    int _panStartX, _panStartY;


    QMenu* menu;

    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* leftRotateAction;
    QAction* rightRotateAction;
    QAction* brushAction;
    QAction* rectangleAction;
    QAction* triangleAction;
    QAction* ellipseAction;
    QAction* moveShapeAction;
    QAction* blendingAction;
    QAction* lengthMeasurementAction;
    QAction* angleMeasurementAction;
    QAction* brightnessAction;
    QAction* darknessAction;
    QAction* sharpenAction;
    QAction* contrastAction;
    QAction* inversionAction;
    QAction* horizontalFlipAction;
    QAction* verticalFlipAction;
    QAction* implantAction;
    QAction* layoutImageClearAction;
    QAction* sourceSizeAction;
    QAction* brushClearActionAction;
    QAction* imageProcessingClearAction;

//    QPixmap* showTeeth;
//    QGraphicsPixmapItem *toothItem;

    void implantGroup();



protected:
    void mousePressEvent(QMouseEvent *event) override;              // 왼쪽 마우스를 눌렀을 때 좌표 위치
    void mouseDoubleClickEvent(QMouseEvent *event) override;        // 왼쪽 마우스 더블 클릭
    void resizeEvent(QResizeEvent * event) override;                // 위젯 창 변경될 때
    //    void changeEvent(QEvent *event) override;                     // 윈도우 창 변경될 때



private:
    void gridLayout();
    void actionGroup();


    int slot_c1;
    int slot_c2;
    int slot_c3;
    int slot_c4;
    int slot_c5;
    int slot_c6;
    int slot_c7;
    int slot_c8;
    int slot_c9;
    int slot_c10;
    int slot_c11;
    int slot_c12;
    int slot_c13;
    int slot_c14;
    int slot_c15;
    int slot_c16;
    int slot_c17;
    int slot_c18;
    int slot_c19;
    int slot_c20;
    int slot_c21;
    int slot_c22;
    int slot_c23;
    int slot_c24;


public slots:
    void slot_keyCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                       int, int, int, int, int, int, int, int, int, int, int, int);

//    /* Top Left Teeth */
//    void slot_11(QPixmap*);
//    void slot_12(QPixmap*);
//    void slot_13(QPixmap*);
//    void slot_14(QPixmap*);
//    void slot_15(QPixmap*);
//    void slot_16(QPixmap*);
//    void slot_17(QPixmap*);

//    /* Top Right Teeth */
//    void slot_21(QPixmap*);
//    void slot_22(QPixmap*);
//    void slot_23(QPixmap*);
//    void slot_24(QPixmap*);
//    void slot_25(QPixmap*);
//    void slot_26(QPixmap*);
//    void slot_27(QPixmap*);

//    /* Bottom Right Teeth */
//    void slot_31(QPixmap*);
//    void slot_32(QPixmap*);
//    void slot_33(QPixmap*);
//    void slot_34(QPixmap*);
//    void slot_35(QPixmap*);
//    void slot_36(QPixmap*);
//    void slot_37(QPixmap*);

//    /* Bottom Left Teeth */
//    void slot_41(QPixmap*);
//    void slot_42(QPixmap*);
//    void slot_43(QPixmap*);
//    void slot_44(QPixmap*);
//    void slot_45(QPixmap*);
//    void slot_46(QPixmap*);
//    void slot_47(QPixmap*);


private slots:
    void showMenu(QPointF clickPoint);



signals:
    void sig_size(QGraphicsView *grid);
    void sig_widgetbyDClick(QGraphicsView *grid);

    void sig_point(QPointF);
    void sig_mouseMiddle(QGraphicsView* grid);

    void sig_teethPoint(QPointF);
};

#endif // LAYOUT_H
