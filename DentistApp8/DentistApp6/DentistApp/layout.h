#ifndef LAYOUT_H
#define LAYOUT_H


#include <QWidget>

class QGraphicsView;
class QGraphicsScene;

class Listalbum;

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


private:
    void gridLayout();





signals:

};

#endif // LAYOUT_H
