#include "layout.h"
#include "view.h"
//#include "scene.h"

#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QPixmap>
#include <QMouseEvent>
#include <QMessageBox>

#include <QPushButton>
#include <QSizePolicy>

#include <QMenu>
#include <QStatusBar>
#include <QSettings>
#include <QGraphicsPixmapItem>
#include <QLabel>




Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
//    key = new KeyFeaturesForm();

    gridLayout();
    actionGroup();

    scene1 = new QGraphicsScene;
    scene1->setBackgroundBrush(Qt::black);
    scene2 = new QGraphicsScene;
    scene2->setBackgroundBrush(Qt::black);
    scene3 = new QGraphicsScene;
    scene3->setBackgroundBrush(Qt::black);
    scene4 = new QGraphicsScene;
    scene4->setBackgroundBrush(Qt::black);

//    scene1 = new Scene;
//    scene1->setBackgroundBrush(Qt::black);
//    scene2 = new Scene;
//    scene2->setBackgroundBrush(Qt::black);
//    scene3 = new Scene;
//    scene3->setBackgroundBrush(Qt::black);
//    scene4 = new Scene;
//    scene4->setBackgroundBrush(Qt::black);


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

    slot_keyCheck(slot_c1, slot_c2, slot_c3, slot_c4, slot_c5, slot_c6,
                  slot_c7, slot_c8, slot_c9, slot_c10, slot_c11, slot_c12,
                  slot_c13, slot_c14, slot_c15, slot_c16, slot_c17, slot_c18,
                  slot_c19, slot_c20, slot_c21, slot_c22, slot_c23, slot_c24);

    connect(this, SIGNAL(sig_point(QPointF)), this, SLOT(showMenu(QPointF)));
}

void Layout::actionGroup()
{
    menu = new QMenu;    // menu : 멤버변수, 메뉴 생성

    zoomInAction = new QAction(tr("&Zoom In"));
    zoomOutAction = new QAction(tr("&Zoom Out"));
    rightRotateAction = new QAction(tr("&90° Rotate"));
    leftRotateAction = new QAction(tr("&-90° Rotate"));
    brushAction = new QAction(tr("&Brush"));
    rectangleAction = new QAction(tr("&Rectangle"));
    triangleAction = new QAction(tr("&Triangle"));
    ellipseAction = new QAction(tr("&Ellipse"));
    moveShapeAction = new QAction(tr("&Move Shape"));
    blendingAction = new QAction(tr("&Blending"));
    lengthMeasurementAction = new QAction(tr("&Length Measurement"));
    angleMeasurementAction = new QAction(tr("&Angle Measurement"));
    brightnessAction = new QAction(tr("&Brightness"));
    darknessAction = new QAction(tr("&Darkness"));
    sharpenAction = new QAction(tr("&Sharpen"));
    contrastAction = new QAction(tr("&Contrast"));
    inversionAction = new QAction(tr("&Inversion"));
    horizontalFlipAction = new QAction(tr("&Horizontal Flip"));
    verticalFlipAction = new QAction(tr("&Vertical Flip"));
    implantAction = new QAction(tr("&Implant"));
    sourceSizeAction = new QAction(tr("&Source Size"));
    layoutImageClearAction = new QAction(tr("&Layout Image Clear"));
    brushClearActionAction = new QAction(tr("&Brush Clear"));
    imageProcessingClearAction = new QAction(tr("&Image Processing"));
}


/* 2 X 2 Grid */
void Layout::gridLayout()
{    
    grid1 = new View;
    grid2 = new View;
    grid3 = new View;
    grid4 = new View;


//    QHBoxLayout *lay1 = new QHBoxLayout;
//    lay1->addWidget(grid1);
//    lay1->addSpacing(5);
//    lay1->addWidget(grid2);

//    QHBoxLayout *lay2 = new QHBoxLayout;
//    lay2->addWidget(grid3);
//    lay2->addSpacing(5);
//    lay2->addWidget(grid4);

//    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->addLayout(lay1);
//    layout->addSpacing(5);
//    layout->addLayout(lay2);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(grid1, 0, 0);
    layout->addWidget(grid2, 0, 1, 1, 1);
    layout->addWidget(grid3, 1, 0, 1, 1);
    layout->addWidget(grid4, 1, 1, 1, 1);
    layout->setSpacing(7);

    for( int i =0 ; i < 2; i++ ) {
        layout->setRowStretch( i, 1);

        for ( int j = 0; j < 2; j++) {
            if( i == 0 ) {
                layout->setColumnStretch( j, 1 );
            }
        }
    }
    setLayout(layout);
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

            grid1->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");

            qDebug("scene1 clicked 1");

            if(t == true){
                qDebug() << "작동하나요?";           // view에서 치아 이미지가 들어갈 위치

                //QGraphicsPixmapItem *toothItem = new QGraphicsPixmapItem(showT11->scaled(20, 40, Qt::KeepAspectRatio));
                //toothItem->setPos(clickPoint);
                //scene->addItem(toothItem);
            }
        }

        /* grid 2번 위치 */
        else if((clickPoint.x() >= x2 && clickPoint.x() <= width2) && (clickPoint.y() >= y2 && clickPoint.y() <= height2)) {
            g = true;
            grid = grid2;
            scene = scene2;

            grid2->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");

            qDebug("scene2 clicked 2");
        }

        /* grid 3번 위치 */
        else if((clickPoint.x() >= x3 && clickPoint.x() <= width3) && (clickPoint.y() >= y3 && clickPoint.y() <= height3)) {
            g = true;
            grid = grid3;
            scene = scene3;

            grid3->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");

            qDebug("scene3 clicked 3");
        }

        /* grid 4번 위치 */
        else if((clickPoint.x() >= x4 && clickPoint.x() <= width4) && (clickPoint.y() >= y4 && clickPoint.y() <= height4)) {
            //grid4->setScene(scene4);
            g = true;
            grid = grid4;
            scene = scene4;

            grid4->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");

            qDebug("scene4 clicked 4");
        }
        qDebug("시그널");
    }

    /* 클릭 단축키 */
    if(event->button() == Qt::MiddleButton) {

        qDebug() << "마우스 가운데 클릭";
        clickPoint = event->pos();

        if((clickPoint.x() >= grid1->pos().x() && clickPoint.x() <= grid1->pos().x() + grid1->width())
                && (clickPoint.y() >= grid1->pos().y() && clickPoint.y() <= grid1->pos().y() + grid1->height())) {
            g = true;
//            grid = grid1;                     // 마우스 가운데 버튼을 클릭한 좌표 위치에 해당하는 view를 선택하여 Action이 적용됨.
            emit sig_point(clickPoint);
        }
        else if((clickPoint.x() >= grid2->pos().x() && clickPoint.x() <= grid2->pos().x() + grid2->width())
                && (clickPoint.y() >= grid2->pos().y() && clickPoint.y() <= grid2->pos().y() + grid2->height())) {
            g = true;
//            grid = grid2;                     // 마우스 가운데 버튼을 클릭한 좌표 위치에 해당하는 view를 선택하여 Action이 적용됨.
            emit sig_point(clickPoint);
        }
        else if((clickPoint.x() >= grid3->pos().x() && clickPoint.x() <= grid3->pos().x() + grid3->width())
                && (clickPoint.y() >= grid3->pos().y() && clickPoint.y() <= grid3->pos().y() + grid3->height())) {
            g = true;
//            grid = grid3;                     // 마우스 가운데 버튼을 클릭한 좌표 위치에 해당하는 view를 선택하여 Action이 적용됨.
            emit sig_point(clickPoint);
        }
        else if((clickPoint.x() >= grid4->pos().x() && clickPoint.x() <= grid4->pos().x() + grid4->width())
                && (clickPoint.y() >= grid4->pos().y() && clickPoint.y() <= grid4->pos().y() + grid4->height())) {
            g = true;
//            grid = grid4;                     // 마우스 가운데 버튼을 클릭한 좌표 위치에 해당하는 view를 선택하여 Action이 적용됨.
            emit sig_point(clickPoint);
        }
    }
}

void Layout::showMenu(QPointF clickPoint)
{
    /* 체크 상태 된 기능만 메뉴바에 생성됨 */
    slot_keyCheck(slot_c1, slot_c2, slot_c3, slot_c4, slot_c5, slot_c6,
                  slot_c7, slot_c8, slot_c9, slot_c10, slot_c11, slot_c12,
                  slot_c13, slot_c14, slot_c15, slot_c16, slot_c17, slot_c18,
                  slot_c19, slot_c20, slot_c21, slot_c22, slot_c23, slot_c24);


    QPoint point = this->mapToGlobal(clickPoint.toPoint());
    menu->exec(point);

    qDebug() << "showMenu" << point;
}


void Layout::slot_11(QPixmap* teeth11)
{
    t = true;

    showTeeth = teeth11;            // 11번 치아 이미지



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

void Layout::slot_keyCheck(int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9, int c10, int c11, int c12,
                           int c13, int c14, int c15, int c16, int c17, int c18, int c19, int c20, int c21, int c22, int c23, int c24)
{
    qDebug() << "찍힘";

    menu->clear();

    if(c1 == 1)
        menu->addAction(zoomInAction);
    if(c2 == 1)
        menu->addAction(zoomOutAction);
    if(c3 == 1)
        menu->addAction(rightRotateAction);
    if(c4 == 1)
        menu->addAction(leftRotateAction);
    if(c5 == 1)
        menu->addAction(brushAction);
    if(c6 == 1)
        menu->addAction(rectangleAction);
    if(c7 == 1)
        menu->addAction(triangleAction);
    if(c8 == 1)
        menu->addAction(ellipseAction);
    if(c9 == 1)
        menu->addAction(moveShapeAction);
    if(c10 == 1)
        menu->addAction(blendingAction);
    if(c11 == 1)
        menu->addAction(lengthMeasurementAction);
    if(c12 == 1)
        menu->addAction(angleMeasurementAction);
    if(c13 == 1)
        menu->addAction(brightnessAction);
    if(c14 == 1)
        menu->addAction(darknessAction);
    if(c15 == 1)
        menu->addAction(sharpenAction);
    if(c16 == 1)
        menu->addAction(contrastAction);
    if(c17 == 1)
        menu->addAction(inversionAction);
    if(c18 == 1)
        menu->addAction(horizontalFlipAction);
    if(c19 == 1)
        menu->addAction(verticalFlipAction);
    if(c20 == 1)
        menu->addAction(implantAction);
    if(c21 == 1)
        menu->addAction(sourceSizeAction);
    if(c22 == 1)
        menu->addAction(layoutImageClearAction);
    if(c23 == 1)
        menu->addAction(brushClearActionAction);
    if(c24 == 1)
        menu->addAction(imageProcessingClearAction);

    slot_c1 = c1;
    slot_c2 = c2;
    slot_c3 = c3;
    slot_c4 = c4;
    slot_c5 = c5;
    slot_c6 = c6;
    slot_c7 = c7;
    slot_c8 = c8;
    slot_c9 = c9;
    slot_c10 = c10;
    slot_c11 = c11;
    slot_c12 = c12;
    slot_c13 = c13;
    slot_c14 = c14;
    slot_c15 = c15;
    slot_c16 = c16;
    slot_c17 = c17;
    slot_c18 = c18;
    slot_c19 = c19;
    slot_c20 = c20;
    slot_c21 = c21;
    slot_c22 = c22;
    slot_c23 = c23;
    slot_c24 = c24;
}



