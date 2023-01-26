#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;
class QPushButton;
//class QHoverEvent;
class QEvent;


class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);

    QGraphicsView *grid1;
    QGraphicsView *grid2;
    QGraphicsView *grid3;
    QGraphicsView *grid4;

    QGraphicsScene *scene1;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;
    QGraphicsScene *scene4;

    QGraphicsView *grid;
    QGraphicsScene *scene;


    QPointF clickPoint;
    bool g = false;

protected:
    void mousePressEvent(QMouseEvent *event) override;              // 왼쪽 마우스를 눌렀을 때 좌표 위치
    void mouseDoubleClickEvent(QMouseEvent *event) override;        // 왼쪽 마우스 더블 클릭
//    void changeEvent(QEvent *event) override;                       // 윈도우 창 변경될 때
    void resizeEvent(QResizeEvent * event) override;                // 위젯 창 변경될 때


private:
    void gridLayout();


signals:
    void sig_size(QGraphicsView *grid);
    void sig_widgetbyDClick(QGraphicsView *grid);


public slots:
};

#endif // LAYOUT_H
