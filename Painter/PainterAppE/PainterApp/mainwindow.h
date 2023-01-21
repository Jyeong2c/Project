#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scene.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    //void calcOrtho(int fx, int fy, int lx, int ly);
    void setFirstX(int &x);
    void setFirstY(int &y);

public slots:
    /*길이 측정 시그널을 받는 슬롯*/
    void receiveFirstOrtho(int _x, int _y);
    void receiveLastOrtho(int _x, int _y);

    /*각도 측정 결과 시그널을 받는 슬롯*/
    void receiveAngleMeasure(double angle);

private slots:
    void on_actionImageLoad_triggered();
    void on_actionBrush_triggered();
    void on_actionRect_triggered();
    void on_actionEllipse_triggered();
    void on_actionLine_triggered();
    void on_actionColor_triggered();
    void on_actionImplantImage_triggered();
    void on_actionClear_triggered();
    void on_actionLength_triggered();
    void on_actionAngle_triggered();

private:
    Ui::MainWindow *ui;

    Scene  * scene;

    int firstX, firstY;
};

#endif // MAINWINDOW_H
