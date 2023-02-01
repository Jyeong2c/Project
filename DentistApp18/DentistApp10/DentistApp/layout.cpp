#include "layout.h"
//#include "view.h"
//#include <opencv2/opencv.hpp>
#include "scene.h"

#include <QGridLayout>
#include <QGraphicsView>

#include <QPixmap>
#include <QMouseEvent>
#include <QMessageBox>

#include <QPushButton>
#include <QSizePolicy>

//#include <QGraphicsSceneEvent>


Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();
    sceneLayout();

    grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");
    grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");
    grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");
    grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");

    grid1->setScene(scene1);
    grid2->setScene(scene2);
    grid3->setScene(scene3);
    grid4->setScene(scene4);

    grid = grid1;
}

/* 2 X 2 Grid */
void Layout::gridLayout()
{
    grid1 = new QGraphicsView;
    grid2 = new QGraphicsView;
    grid3 = new QGraphicsView;
    grid4 = new QGraphicsView;


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
}

void Layout::sceneLayout()
{
    scene1 = new Scene;
    scene1->setBackgroundBrush(Qt::black);
    scene2 = new Scene;
    scene2->setBackgroundBrush(Qt::black);
    scene3 = new Scene;
    scene3->setBackgroundBrush(Qt::black);
    scene4 = new Scene;
    scene4->setBackgroundBrush(Qt::black);
    scene = new Scene;
    scene->setBackgroundBrush(Qt::black);
}


void Layout::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Mouse Pressed" << event->pos();

    if(event->button() == Qt::LeftButton) {
        clickPoint = event->pos();

        int x1 = grid1->pos().x();                 // grid1의 topLeft의 x 좌표값
        int y1 = grid1->pos().y();                 // grid1의 topLeft의 y 좌표값
        int width1 = x1 + grid1->width();          // grid1의 topLeft의 x좌표값에서 grid의 폭만큼 이동한 x의 좌표값
        int height1 = y1 + grid1->height();        // grid1의 topLeft의 y 좌표값에서 grid의 높이만큼 이동한 y의 좌표값

        /* grid2(graphicsView) 의 좌표값 */
        int x2 = grid2->pos().x();
        int y2 = grid2->pos().y();
        int width2 = x2 + grid2->width();
        int height2 = y2 + grid2->height();

        /* grid3(graphicsView) 의 좌표값 */
        int x3 = grid3->pos().x();
        int y3 = grid3->pos().y();
        int width3 = x3 + grid3->width();
        int height3 = y3 + grid3->height();

        /* grid4(graphicsView) 의 좌표값 */
        int x4 = grid4->pos().x();
        int y4 = grid4->pos().y();
        int width4 = x4 + grid4->width();
        int height4 = y4 + grid4->height();

        /* grid 1번 위치 */
        if((clickPoint.x() >= x1 && clickPoint.x() <= width1) && (clickPoint.y() >= y1 && clickPoint.y() <= height1)) {
            g = true;
            grid = grid1;
            scene = scene1;
            qDebug("scene1 clicked 1");
        }

        /* grid 2번 위치 */
        else if((clickPoint.x() >= x2 && clickPoint.x() <= width2) && (clickPoint.y() >= y2 && clickPoint.y() <= height2)) {
            g = true;
            grid = grid2;
            scene = scene2;

            qDebug("scene2 clicked 2");
        }

        /* grid 3번 위치 */
        else if((clickPoint.x() >= x3 && clickPoint.x() <= width3) && (clickPoint.y() >= y3 && clickPoint.y() <= height3)) {
            g = true;
            grid = grid3;
            scene = scene3;

            qDebug("scene3 clicked 3");
        }

        /* grid 4번 위치 */
        else if((clickPoint.x() >= x4 && clickPoint.x() <= width4) && (clickPoint.y() >= y4 && clickPoint.y() <= height4)) {
            //grid4->setScene(scene4);
            g = true;
            grid = grid4;
            scene = scene4;

            qDebug("scene4 clicked 4");
        }
        qDebug("시그널");
    }
}

void Layout::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "Mouse mouseDoubleClickEvent" << event->pos();

    if(event->button() == Qt::LeftButton) {
        clickPoint = event->pos();

        if((clickPoint.x() >= grid1->pos().x() && clickPoint.x() <= grid1->pos().x() + grid1->width())
                && (clickPoint.y() >= grid1->pos().y() && clickPoint.y() <= grid1->pos().y() + grid1->height())) {

            grid = grid1;
            emit sig_widgetbyDClick(grid);

            qDebug() << "Double check1";
        }
        else if((clickPoint.x() >= grid2->pos().x() && clickPoint.x() <= grid2->pos().x() + grid2->width())
                && (clickPoint.y() >= grid2->pos().y() && clickPoint.y() <= grid2->pos().y() + grid2->height())) {

            grid = grid2;
            emit sig_widgetbyDClick(grid);

            qDebug() << "Double check2";
        }
        else if((clickPoint.x() >= grid3->pos().x() && clickPoint.x() <= grid3->pos().x() + grid3->width())
                && (clickPoint.y() >= grid3->pos().y() && clickPoint.y() <= grid3->pos().y() + grid3->height())) {

            grid = grid3;
            emit sig_widgetbyDClick(grid);

            qDebug() << "Double check3";
        }
        else if((clickPoint.x() >= grid4->pos().x() && clickPoint.x() <= grid4->pos().x() + grid4->width())
                && (clickPoint.y() >= grid4->pos().y() && clickPoint.y() <= grid4->pos().y() + grid4->height())) {

            grid = grid4;
            emit sig_widgetbyDClick(grid);

            qDebug() << "Double check4";
        }
    }
}


void Layout::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    emit sig_size(grid);
}
