#include "layout.h"

#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>

#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)
Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();

    image = new QImage();
//    imgSize->load("C:/Users/KOSA/Desktop/HTML Canvas/HTML Canvas/orthopan.jpg");
    image->load("C:/Users/KOSA/Desktop/Qt/tooth.png");

    QPixmap buf = QPixmap::fromImage(*image);
    buf = buf.scaled(image->width(), image->height());

    scene = new QGraphicsScene;
    scene->addPixmap(buf);
    grid1->setScene(scene);
    //grid1->setBackgroundBrush(Qt::black);
}

/* 2 X 2 Grid */
void Layout::gridLayout()
{
    //QWidget *window = new QWidget;

    grid1 = new QGraphicsView;
    QGraphicsView *grid2 = new QGraphicsView;
    QGraphicsView *grid3 = new QGraphicsView;
    QGraphicsView *grid4 = new QGraphicsView;

//    QGridLayout *layout = new QGridLayout(this);
//    layout->addWidget(grid1, 0, 0);                   // QWidget*, row, column->addSpacing(5);
//    layout->addWidget(grid2, 0, 1);
//    layout->addWidget(grid3, 1, 0);
//    layout->addWidget(grid4, 1, 1);

    QHBoxLayout *lay1 = new QHBoxLayout;
    lay1->addWidget(grid1);
    lay1->addSpacing(5);
    lay1->addWidget(grid2);

    QHBoxLayout *lay2 = new QHBoxLayout;
    lay2->addWidget(grid3);
    lay2->addSpacing(5);
    lay2->addWidget(grid4);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(lay1);
    layout->addSpacing(5);
    layout->addLayout(lay2);

    //layout->addWidget(bigButton, 2, 0, 1, 2);
}
void Layout::bright()
{
    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
            const QRgb rgb = image->pixel(x, y);
            const double r =LIMIT_UBYTE(qRed(rgb)   + 5);
            const double g =LIMIT_UBYTE(qGreen(rgb) + 5);
            const double b =LIMIT_UBYTE(qBlue(rgb)  + 5);

            image->setPixelColor(x, y,QColor(r,g,b));
    }
    scene = new QGraphicsScene;

    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);
}

void Layout::bright2()
{
    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
            const QRgb rgb = image->pixel(x, y);
            const double r =LIMIT_UBYTE(qRed(rgb)   - 5);
            const double g =LIMIT_UBYTE(qGreen(rgb) - 5);
            const double b =LIMIT_UBYTE(qBlue(rgb)  - 5);

            image->setPixelColor(x, y,QColor(r,g,b));
//            image->invertPixels(255 - );
    }
    scene = new QGraphicsScene;

    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);
}
