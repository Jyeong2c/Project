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

    View *grid1;
    View *grid2;
    View *grid3;
    View *grid4;


    Scene *scene1;
    Scene *scene2;
    Scene *scene3;
    Scene *scene4;


    View *m_currentView;
    Scene *m_currentScene;


    QPointF clickPoint;
    bool g = false;
    bool s = false;

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
    QAction* cursorAction;
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
    QAction* drawClearAction;
    QAction* implantClearAction;
    QAction* imageProcessingClearAction;
    QAction* arrowAction;
    QAction* lineAction;



protected:
    void mousePressEvent(QMouseEvent *event) override;              // 왼쪽 마우스를 눌렀을 때 좌표 위치
    void mouseDoubleClickEvent(QMouseEvent *event) override;        // 왼쪽 마우스 더블 클릭
    void resizeEvent(QResizeEvent * event) override;                // 위젯 창 변경될 때


private:
    void gridLayout();
    void actionGroup();


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
    void showMenu(QPointF clickPoint);



signals:
    void sig_widgetbyDClick();


    void sig_point(QPointF);
    void sig_mouseMiddle(QGraphicsView* grid);

    void sig_teethPoint(QPointF);

    void sig_cursorChange();
};

#endif // LAYOUT_H
