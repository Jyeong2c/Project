#ifndef LAYOUT_H
#define LAYOUT_H

#include "scene.h"
#include <QWidget>

class QGraphicsScene;
class QGraphicsView;

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);
    QGraphicsView *grid1;
 Scene * scene;
public slots:
    void brightUp();
    void brightnessLow();
    void inverstion();
    void verticalFlip();
    void horizontalFlip();
    void sharpen();
    void zoomIn();
    void zoomOut();
    void leftRotate();
    void rightRotate();

    void brightnessUpOpenCV();
    void brightnessDownOpenCV();
    void brightSlider(int);
    void his();
    void imageClear();


private:
    void gridLayout();
    QImage * m_image;
    QImage * m_copyImage;

    QGraphicsView *grid2;
//    QGraphicsScene* scene;
//    Scene *scene ;
//    QImage *image_to_display;



    int intensity=0;
    int inten=0;
    int cnt=0;

signals:

};

#endif // LAYOUT_H
