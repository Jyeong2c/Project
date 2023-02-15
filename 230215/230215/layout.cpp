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
    gridLayout();   // Layout(view1, 2, 3, 4)을 생성하는 함수
    actionGroup();   // Key Features에 사용되는 Action들을 모아논 함수

    scene1 = new Scene;   // view1에 들어가는 scene1 할당
    scene1->setBackgroundBrush(Qt::black);   // scene2의 배경을 검은색으로 설정
    scene2 = new Scene;   // view2에 들어가는 scene2 할당
    scene2->setBackgroundBrush(Qt::black);   // scene2의 배경을 검은색으로 설정
    scene3 = new Scene;   // view3에 들어가는 scene3 할당
    scene3->setBackgroundBrush(Qt::black);   // scene2의 배경을 검은색으로 설정
    scene4 = new Scene;   // view4에 들어가는 scene4 할당
    scene4->setBackgroundBrush(Qt::black);   // scene2의 배경을 검은색으로 설정

    m_currentView = new View;   // 선택된 view를 담는 현재 m_currentView 할당
    m_currentScene = new Scene;   // 선택된 scene을 담는 m_currentScene 할당

    grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /* view1, 2, 3, 4의 테두리 굵기, 선 종류, 색상 */
    grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");
    grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");
    grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");
    grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143);");

    grid1->setScene(scene1);
    grid2->setScene(scene2);
    grid3->setScene(scene3);
    grid4->setScene(scene4);

    m_currentView = grid1;

    connect(this, SIGNAL(sig_point(QPointF)), this, SLOT(showMenu(QPointF)));   // 마우스 가운데 버튼을 클릭한 좌표에 showMenu 함수 출력
}

/* Key Features 기능에 필요한 Action들을 할당한 함수 */
void Layout::actionGroup()
{
    menu = new QMenu;

    zoomInAction = new QAction(tr("&Zoom In")); // 확대 Action 할당
    zoomOutAction = new QAction(tr("&Zoom Out"));   // 축소 Action 할당
    rightRotateAction = new QAction(tr("&90° Rotate")); // 90°회전 Action 할당
    leftRotateAction = new QAction(tr("&-90° Rotate")); // -90°회전 Action 할당
    brushAction = new QAction(tr("&Brush"));    // 브러쉬 Action 할당
    rectangleAction = new QAction(tr("&Rectangle"));    // 사각형 Action 할당
    triangleAction = new QAction(tr("&Triangle"));  // 삼각형 Action 할당
    ellipseAction = new QAction(tr("&Ellipse"));    // 원형 Action 할당
    cursorAction = new QAction(tr("&Cursor"));  // 커서 Action 할당
    blendingAction = new QAction(tr("&Blending"));  // 블랜딩 Action 할당
    lengthMeasurementAction = new QAction(tr("&Length Measurement"));   // 길이 측정 Action 할당
    angleMeasurementAction = new QAction(tr("&Angle Measurement")); // 각도 측정 Action 할당
    brightnessAction = new QAction(tr("&Brightness"));  // 밝기 Action 할당
    darknessAction = new QAction(tr("&Darkness"));  // 어둡기 Action 할당
    sharpenAction = new QAction(tr("&Sharpen"));    // 선명화 Action 할당
    contrastAction = new QAction(tr("&Contrast"));  // 명암 Action 할당
    inversionAction = new QAction(tr("&Inversion"));    // 대비 Action 할당
    horizontalFlipAction = new QAction(tr("&Horizontal Flip")); // 수평 반전 Action 할당
    verticalFlipAction = new QAction(tr("&Vertical Flip")); // 수직 반전 Action 할당
    implantAction = new QAction(tr("&Implant"));    // 임플란트 Action 할당
    sourceSizeAction = new QAction(tr("&Source Size")); // 원본 크기 Action 할당
    layoutImageClearAction = new QAction(tr("&Layout Image Clear"));    // 레이아웃 초기화 Action 할당
    drawClearAction = new QAction(tr("&Draw Clear"));   // Draw 초기화 Action 할당
    implantClearAction = new QAction(tr("&Implant Clear")); // 임플란트 초기화 Action 할당
    imageProcessingClearAction = new QAction(tr("&Image Processing"));  // 이미지 프로세싱 초기화 Action 할당
    arrowAction = new QAction(tr("&Arrow"));    // 화살표 Action 할당
    lineAction = new QAction(tr("&Line"));  // 선 Action 할당
}


/* 2 X 2 Layout */
void Layout::gridLayout()
{    
    grid1 = new View;   // view1 할당
    grid2 = new View;   // view2 할당
    grid3 = new View;   // view3 할당
    grid4 = new View;   // view4 할당

    QGridLayout *layout = new QGridLayout(this);    // Layout 할당
    layout->addWidget(grid1, 0, 0);    // Layout 0행 0열 위치에 grid1(view1) 삽입
    layout->addWidget(grid2, 0, 1, 1, 1);   // Layout 0행 1열 위치에 grid2(view2) 삽입
    layout->addWidget(grid3, 1, 0, 1, 1);   // Layout 1행 0열 위치에 grid3(view3) 삽입
    layout->addWidget(grid4, 1, 1, 1, 1);   // Layout 1행 1열 위치에 grid4(view4) 삽입
    layout->setSpacing(7);  // Layout 간격

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

/* 마우스를 클릭했을 때 실행하는 함수 override */
void Layout::mousePressEvent(QMouseEvent *event)
{
    /* 마우스 왼쪽 버튼을 클릭한 경우 */
    if(event->button() == Qt::LeftButton) {
        clickPoint = event->pos();

        int x1 = grid1->pos().x();  // grid1의 topLeft의 x 좌표값
        int y1 = grid1->pos().y();  // grid1의 topLeft의 y 좌표값
        int width1 = x1 + grid1->width();   // grid1의 topLeft의 x좌표값에서 grid의 폭만큼 이동한 x의 좌표값
        int height1 = y1 + grid1->height(); // grid1의 topLeft의 y 좌표값에서 grid의 높이만큼 이동한 y의 좌표값

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

        /* grid 1번 위치에 마우스가 클릭된 경우 */
        if((clickPoint.x() >= x1 && clickPoint.x() <= width1) && (clickPoint.y() >= y1 && clickPoint.y() <= height1)) {
            g = true;
            m_currentView = grid1;
            m_currentScene = scene1;

            grid1->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        /* grid 2번 위치에 마우스가 클릭된 경우 */
        else if((clickPoint.x() >= x2 && clickPoint.x() <= width2) && (clickPoint.y() >= y2 && clickPoint.y() <= height2)) {
            g = true;
            m_currentView = grid2;
            m_currentScene = scene2;

            grid2->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        /* grid 3번 위치에 마우스가 클릭된 경우 */
        else if((clickPoint.x() >= x3 && clickPoint.x() <= width3) && (clickPoint.y() >= y3 && clickPoint.y() <= height3)) {
            g = true;
            m_currentView = grid3;
            m_currentScene = scene3;

            grid3->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        /* grid 4번 위치에 마우스가 클릭된 경우 */
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

    /* 마우스 가운데 버튼을 클릭한 경우 - Key Features 기능들 작동 */
    if(event->button() == Qt::MiddleButton) {
        clickPoint = event->pos();

        if((clickPoint.x() >= grid1->pos().x() && clickPoint.x() <= grid1->pos().x() + grid1->width())
                && (clickPoint.y() >= grid1->pos().y() && clickPoint.y() <= grid1->pos().y() + grid1->height())) {
            g = true;
            emit sig_point(clickPoint); // 현재 좌표 위치를 시그널로 전달
        }
        else if((clickPoint.x() >= grid2->pos().x() && clickPoint.x() <= grid2->pos().x() + grid2->width())
                && (clickPoint.y() >= grid2->pos().y() && clickPoint.y() <= grid2->pos().y() + grid2->height())) {
            g = true;
            emit sig_point(clickPoint); // 현재 좌표 위치를 시그널로 전달
        }
        else if((clickPoint.x() >= grid3->pos().x() && clickPoint.x() <= grid3->pos().x() + grid3->width())
                && (clickPoint.y() >= grid3->pos().y() && clickPoint.y() <= grid3->pos().y() + grid3->height())) {
            g = true;
            emit sig_point(clickPoint); // 현재 좌표 위치를 시그널로 전달
        }
        else if((clickPoint.x() >= grid4->pos().x() && clickPoint.x() <= grid4->pos().x() + grid4->width())
                && (clickPoint.y() >= grid4->pos().y() && clickPoint.y() <= grid4->pos().y() + grid4->height())) {
            g = true;
            emit sig_point(clickPoint); // 현재 좌표 위치를 시그널로 전달
        }
    }

/* EUN JI */
//--------------------------------------------------------------------
    if(event->button() == Qt::RightButton){
       emit sig_cursorChange();
    }
}

/* 마우스를 더블 클릭했을 때 실행하는 함수 override */
void Layout::mouseDoubleClickEvent(QMouseEvent *event)
{
    /* 마우스 왼쪽 버튼을 더블 클릭한 경우 */
    if(event->button() == Qt::LeftButton) {
        clickPoint = event->pos();

        /* grid 1번 위치에 마우스가 더블 클릭된 경우 */
        if((clickPoint.x() >= grid1->pos().x() && clickPoint.x() <= grid1->pos().x() + grid1->width())
                && (clickPoint.y() >= grid1->pos().y() && clickPoint.y() <= grid1->pos().y() + grid1->height())) {
            m_currentView = grid1;
            emit sig_widgetbyDClick();  // 마우스를 더블 클릭했을 때 발생하는 시그널
        }

        /* grid 2번 위치에 마우스가 더블 클릭된 경우 */
        else if((clickPoint.x() >= grid2->pos().x() && clickPoint.x() <= grid2->pos().x() + grid2->width())
                && (clickPoint.y() >= grid2->pos().y() && clickPoint.y() <= grid2->pos().y() + grid2->height())) {
            m_currentView = grid2;
            emit sig_widgetbyDClick();  // 마우스를 더블 클릭했을 때 발생하는 시그널
        }

        /* grid 3번 위치에 마우스가 더블 클릭된 경우 */
        else if((clickPoint.x() >= grid3->pos().x() && clickPoint.x() <= grid3->pos().x() + grid3->width())
                && (clickPoint.y() >= grid3->pos().y() && clickPoint.y() <= grid3->pos().y() + grid3->height())) {
            m_currentView = grid3;
            emit sig_widgetbyDClick();  // 마우스를 더블 클릭했을 때 발생하는 시그널
        }

        /* grid 4번 위치에 마우스가 더블 클릭된 경우 */
        else if((clickPoint.x() >= grid4->pos().x() && clickPoint.x() <= grid4->pos().x() + grid4->width())
                && (clickPoint.y() >= grid4->pos().y() && clickPoint.y() <= grid4->pos().y() + grid4->height())) {
            m_currentView = grid4;
            emit sig_widgetbyDClick();  // 마우스를 더블 클릭했을 때 발생하는 시그널
        }
    }
}

/* Layout의 크기가 변경될 때 실행하는 함수 override */
void Layout::resizeEvent(QResizeEvent* event)
{
    /* Layout의 이전 가로폭이 0보다 클 경우 */
    if(event->oldSize().width() > 0) {

        /* Layout의 크기가 변경될 때 grid1의 가로 세로 크기를 다시 설정해 줌 */
        grid1->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());
        grid1->fitInView(scene1->sceneRect(), Qt::KeepAspectRatio);
        grid1->viewport()->update();

        /* Layout의 크기가 변경될 때 grid2의 가로 세로 크기를 다시 설정해 줌 */
        grid2->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());
        grid2->fitInView(scene2->sceneRect(), Qt::KeepAspectRatio);
        grid2->viewport()->update();

        /* Layout의 크기가 변경될 때 grid3의 가로 세로 크기를 다시 설정해 줌 */
        grid3->scale(event->size().width()/(float)event->oldSize().width(),
                     event->size().height()/(float)event->oldSize().height());
        grid3->fitInView(scene3->sceneRect(), Qt::KeepAspectRatio);
        grid3->viewport()->update();

        /* Layout의 크기가 변경될 때 grid4의 가로 세로 크기를 다시 설정해 줌 */
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

/* 인자로 받은 save를 s 멤버변수에 저장하는 슬롯 */
void Layout::slot_save(bool save)
{
    s = save;
}

/* Action을 실행하는 슬롯 */
void Layout::showMenu(QPointF clickPoint)
{
    /* 처음 프로그램을 실행했을 때, 로그아웃했을 때 DB에 저장된 체크 상태를 메뉴에 출력함 */
    if(s == false) {
    slot_logCheck(log_c1, log_c2, log_c3, log_c4, log_c5, log_c6,
                  log_c7, log_c8, log_c9, log_c10, log_c11, log_c12,
                  log_c13, log_c14, log_c15, log_c16, log_c17, log_c18,
                  log_c19, log_c20, log_c21, log_c22, log_c23, log_c24, log_c25, log_c26, log_c27);
    }
    /* savePushButton을 눌렀을 때 체크 상태 된 기능만 메뉴에 출력함 */
    else if(s == true) {
    slot_keyCheck(slot_c1, slot_c2, slot_c3, slot_c4, slot_c5, slot_c6,
                  slot_c7, slot_c8, slot_c9, slot_c10, slot_c11, slot_c12,
                  slot_c13, slot_c14, slot_c15, slot_c16, slot_c17, slot_c18,
                  slot_c19, slot_c20, slot_c21, slot_c22, slot_c23, slot_c24, slot_c25, slot_c26, slot_c27);
    }
    QPoint point = this->mapToGlobal(clickPoint.toPoint());
    menu->exec(point);  // 메뉴 실행
}

void Layout::slot_keyCheck(int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9, int c10, int c11, int c12,
                           int c13, int c14, int c15, int c16, int c17, int c18, int c19, int c20, int c21, int c22, int c23, int c24, int c25, int c26, int c27)
{
    menu->clear();  // 메뉴 초기화

    /* 메개변수가 1인 경우 메뉴에 해당 Action을 추가 */
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

    /* 메개변수를 멤버변수에 저장 */
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
    menu->clear();  // 메뉴 초기화

    /* 메개변수가 1인 경우 메뉴에 해당 Action을 추가 */
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

    /* 메개변수를 멤버변수에 저장 */
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

