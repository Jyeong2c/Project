#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);

public slots:
    void brightUp();
    void brightnessLow();
    void inverstion();
    void verticalFlip();
    void horizontalFlip();
    void dark();
//    void dark1();
//    void dark2();
    void zoomIn();
    void zoomOut();
    void leftRotate();
    void rightRotate();
    void mouseWheel();
    void imageClear();
    void back();
    void test();
    void test1();


private:
    void gridLayout();
    QImage * image;
    QGraphicsView *grid1;
    QGraphicsView *grid2;
    QGraphicsScene* scene;
    QImage *phaseOneImage;



    int intensity=0;
    int inten=0;
    int cnt=0;

signals:

};

#endif // LAYOUT_H
