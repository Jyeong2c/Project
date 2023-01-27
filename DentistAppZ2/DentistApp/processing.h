#ifndef PROCESSING_H
#define PROCESSING_H

#include <QWidget>
//#include "scene.h"

class QGraphicsScene;
class QGraphicsView;
class Layout;

class Processing : public QWidget
{
    Q_OBJECT
public:
    explicit Processing(QWidget *parent = nullptr);
    QGraphicsView *grid1;
public slots:
    void brightUp();
    void brightnessLow();
    void inverstion(QPixmap item);
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


    QGraphicsScene* scene;
    Layout *customLayout;

//    Scene * ;
//    QImage *image_to_display;



    int intensity=0;
    int inten=0;
    int cnt=0;

signals:


};

#endif // PROCESSING_H


