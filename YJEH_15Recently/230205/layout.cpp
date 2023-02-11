#include "layout.h"
#include "view.h"
#include "scene.h"

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
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QGraphicsSceneMouseEvent>



Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();
    actionGroup();

    scene1 = new Scene;
    scene1->setObjectName("Scene1");
    scene1->setBackgroundBrush(Qt::black);
    scene2 = new Scene;
    scene2->setObjectName("Scene2");
    scene2->setBackgroundBrush(Qt::black);
    scene3 = new Scene;
    scene3->setObjectName("Scene3");
    scene3->setBackgroundBrush(Qt::black);
    scene4 = new Scene;
    scene4->setObjectName("Scene4");
    scene4->setBackgroundBrush(Qt::black);

    m_currentView = new View;
    m_currentScene = new Scene;

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

    m_currentView = grid1;

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
    cursorAction = new QAction(tr("&Cursor"));
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
    drawClearAction = new QAction(tr("&Draw Clear"));
    implantClearAction = new QAction(tr("&Implant Clear"));
    imageProcessingClearAction = new QAction(tr("&Image Processing"));
    arrowAction = new QAction(tr("&Arrow"));
    lineAction = new QAction(tr("&Line"));
}


/* 2 X 2 Grid */
void Layout::gridLayout()
{    
    grid1 = new View;
    grid2 = new View;
    grid3 = new View;
    grid4 = new View;

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
            m_currentView = grid1;
            m_currentScene = scene1;

            grid1->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        /* grid 2번 위치 */
        else if((clickPoint.x() >= x2 && clickPoint.x() <= width2) && (clickPoint.y() >= y2 && clickPoint.y() <= height2)) {
            g = true;
            m_currentView = grid2;
            m_currentScene = scene2;

            grid2->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        /* grid 3번 위치 */
        else if((clickPoint.x() >= x3 && clickPoint.x() <= width3) && (clickPoint.y() >= y3 && clickPoint.y() <= height3)) {
            g = true;
            m_currentView = grid3;
            m_currentScene = scene3;

            grid3->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        /* grid 4번 위치 */
        else if((clickPoint.x() >= x4 && clickPoint.x() <= width4) && (clickPoint.y() >= y4 && clickPoint.y() <= height4)) {
            g = true;
            m_currentView = grid4;
            m_currentScene = scene4;

            grid4->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }
    }

    /* 클릭 단축키 - 마우스 가운데 클릭 */
    if(event->button() == Qt::MiddleButton) {
        clickPoint = event->pos();

        if((clickPoint.x() >= grid1->pos().x() && clickPoint.x() <= grid1->pos().x() + grid1->width())
                && (clickPoint.y() >= grid1->pos().y() && clickPoint.y() <= grid1->pos().y() + grid1->height())) {
            g = true;
            emit sig_point(clickPoint);
        }
        else if((clickPoint.x() >= grid2->pos().x() && clickPoint.x() <= grid2->pos().x() + grid2->width())
                && (clickPoint.y() >= grid2->pos().y() && clickPoint.y() <= grid2->pos().y() + grid2->height())) {
            g = true;
            emit sig_point(clickPoint);
        }
        else if((clickPoint.x() >= grid3->pos().x() && clickPoint.x() <= grid3->pos().x() + grid3->width())
                && (clickPoint.y() >= grid3->pos().y() && clickPoint.y() <= grid3->pos().y() + grid3->height())) {
            g = true;
            emit sig_point(clickPoint);
        }
        else if((clickPoint.x() >= grid4->pos().x() && clickPoint.x() <= grid4->pos().x() + grid4->width())
                && (clickPoint.y() >= grid4->pos().y() && clickPoint.y() <= grid4->pos().y() + grid4->height())) {
            g = true;
            emit sig_point(clickPoint);
        }
    }

/* EUN JI */
//--------------------------------------------------------------------
    if(event->button() == Qt::RightButton){
       emit sig_cursorChange();
    }
}

void Layout::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "Mouse mouseDoubleClickEvent" << event->pos();

    if(event->button() == Qt::LeftButton) {
        clickPoint = event->pos();

        if((clickPoint.x() >= grid1->pos().x() && clickPoint.x() <= grid1->pos().x() + grid1->width())
                && (clickPoint.y() >= grid1->pos().y() && clickPoint.y() <= grid1->pos().y() + grid1->height())) {

            m_currentView = grid1;
            emit sig_widgetbyDClick();
        }

        else if((clickPoint.x() >= grid2->pos().x() && clickPoint.x() <= grid2->pos().x() + grid2->width())
                && (clickPoint.y() >= grid2->pos().y() && clickPoint.y() <= grid2->pos().y() + grid2->height())) {

            m_currentView = grid2;
            emit sig_widgetbyDClick();
        }

        else if((clickPoint.x() >= grid3->pos().x() && clickPoint.x() <= grid3->pos().x() + grid3->width())
                && (clickPoint.y() >= grid3->pos().y() && clickPoint.y() <= grid3->pos().y() + grid3->height())) {

            m_currentView = grid3;
            emit sig_widgetbyDClick();
        }

        else if((clickPoint.x() >= grid4->pos().x() && clickPoint.x() <= grid4->pos().x() + grid4->width())
                && (clickPoint.y() >= grid4->pos().y() && clickPoint.y() <= grid4->pos().y() + grid4->height())) {

            m_currentView = grid4;
            emit sig_widgetbyDClick();
        }
    }
}


void Layout::resizeEvent(QResizeEvent* event)
{
    if(event->oldSize().width() > 0) {
        grid1->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());

        qDebug() << "11size().width()" << event->size().width();
        qDebug() << "11oldSize().width()" << event->oldSize().width();

        grid1->fitInView(scene1->sceneRect(), Qt::KeepAspectRatio);
        grid1->viewport()->update();
        grid2->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());
        grid2->fitInView(scene2->sceneRect(), Qt::KeepAspectRatio);
        grid2->viewport()->update();
        grid3->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());
        grid3->fitInView(scene3->sceneRect(), Qt::KeepAspectRatio);
        grid3->viewport()->update();
        grid4->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());
        grid4->fitInView(scene4->sceneRect(), Qt::KeepAspectRatio);
        grid4->viewport()->update();
    }

    grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void Layout::slot_save(bool save)
{
    s = save;
}

void Layout::showMenu(QPointF clickPoint)
{
    /* 처음 프로그램을 실행했을 때, 로그아웃했을 때 DB에 저장된 체크 상태를 메뉴바에 출력함. */
    if(s == false) {
    slot_logCheck(log_c1, log_c2, log_c3, log_c4, log_c5, log_c6,
                  log_c7, log_c8, log_c9, log_c10, log_c11, log_c12,
                  log_c13, log_c14, log_c15, log_c16, log_c17, log_c18,
                  log_c19, log_c20, log_c21, log_c22, log_c23, log_c24, log_c25, log_c26, log_c27);
    }
    /* save 버튼을 눌렀을 때 체크 상태 된 기능만 메뉴바에 생성됨 */
    else if(s == true) {
    slot_keyCheck(slot_c1, slot_c2, slot_c3, slot_c4, slot_c5, slot_c6,
                  slot_c7, slot_c8, slot_c9, slot_c10, slot_c11, slot_c12,
                  slot_c13, slot_c14, slot_c15, slot_c16, slot_c17, slot_c18,
                  slot_c19, slot_c20, slot_c21, slot_c22, slot_c23, slot_c24, slot_c25, slot_c26, slot_c27);
    }

    QPoint point = this->mapToGlobal(clickPoint.toPoint());
    menu->exec(point);
}

void Layout::slot_keyCheck(int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9, int c10, int c11, int c12,
                           int c13, int c14, int c15, int c16, int c17, int c18, int c19, int c20, int c21, int c22, int c23, int c24, int c25, int c26, int c27)
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
        menu->addAction(cursorAction);
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
        menu->addAction(drawClearAction);
    if(c24 == 1)
        menu->addAction(implantClearAction);
    if(c25 == 1)
        menu->addAction(imageProcessingClearAction);
    if(c26 == 1)
        menu->addAction(arrowAction);
    if(c27 == 1)
        menu->addAction(lineAction);

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
    slot_c25 = c25;
    slot_c26 = c26;
    slot_c27 = c27;
}

void Layout::slot_logCheck(int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9, int c10, int c11, int c12,
                           int c13, int c14, int c15, int c16, int c17, int c18, int c19, int c20, int c21, int c22, int c23, int c24, int c25, int c26, int c27)
{
    menu->clear();

    qDebug() << "찍힘2";
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
        menu->addAction(cursorAction);
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
        menu->addAction(drawClearAction);
    if(c24 == 1)
        menu->addAction(implantClearAction);
    if(c25 == 1)
        menu->addAction(imageProcessingClearAction);
    if(c26 == 1)
        menu->addAction(arrowAction);
    if(c27 == 1)
        menu->addAction(lineAction);

    log_c1 = c1;
    log_c2 = c2;
    log_c3 = c3;
    log_c4 = c4;
    log_c5 = c5;
    log_c6 = c6;
    log_c7 = c7;
    log_c8 = c8;
    log_c9 = c9;
    log_c10 = c10;
    log_c11 = c11;
    log_c12 = c12;
    log_c13 = c13;
    log_c14 = c14;
    log_c15 = c15;
    log_c16 = c16;
    log_c17 = c17;
    log_c18 = c18;
    log_c19 = c19;
    log_c20 = c20;
    log_c21 = c21;
    log_c22 = c22;
    log_c23 = c23;
    log_c24 = c24;
    log_c25 = c25;
    log_c26 = c26;
    log_c27 = c27;
}




