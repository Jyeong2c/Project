#include "maxlayout.h"
#include "view.h"
#include "scene.h"

#include <QGraphicsView>
#include <QBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>

Maxlayout::Maxlayout(QWidget *parent)
    : QWidget{parent}
{
    maxWidget = new QWidget();  // QWidget 할당
    maxNewGrid = new View;  // maxNewGrid 할당
    maxLayBox = new QHBoxLayout(this);
    maxLayBox->addWidget(maxNewGrid);
    maxLayBox->setContentsMargins(0,0,0,0);
    maxNewSc = new Scene;   // maxNewSc 할당
    maxNewGrid->setScene(maxNewSc);

    viewQuit = new QPushButton("X", this);  // viewQuit 할당
    viewQuit->setStyleSheet("background-color: rgb(231, 230, 230);"
                            "border : 2px solid rgb(0, 0, 0);");
}

/* Maxlayout의 크기가 변경될 때 실행하는 함수 override */
void Maxlayout::resizeEvent(QResizeEvent* event)
{
    /* Maxlayout의 이전 가로폭이 0보다 클 경우 */
    if(event->oldSize().width() > 0) {
        maxNewGrid->scale(event->size().width()/(float)event->oldSize().width(),
                          event->size().height()/(float)event->oldSize().height());
        maxNewGrid->fitInView(maxNewSc->sceneRect(),Qt::KeepAspectRatio);
        maxNewGrid->viewport()->update();
    }

    maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    viewQuit->setGeometry(maxNewGrid->width()-45, 10, 30, 30);
}

/* slot_doubleSize에서 실행되는 함수 */
void Maxlayout::setNewSc(Scene* sc)
{
    maxNewSc = sc;
    maxNewGrid->setScene(maxNewSc);
}

void Maxlayout::setNewGrid(View* view)
{
    maxNewGrid = view;
}

/* 마우스를 클릭했을 때 실행하는 함수 override */
void Maxlayout::mousePressEvent(QMouseEvent *event)
{
    /* 마우스 가운데 버튼을 클릭한 경우 - Key Features 기능들 작동 */
    if(event->button() == Qt::MiddleButton) {
        clickPoint = event->pos();
        emit sig_clicked(clickPoint);   // 현재 좌표 위치를 시그널로 전달
    }
}

/* Top Left Teeth */
void Maxlayout::slotD_11(QPixmap* teeth11)
{
    showTeeth = teeth11;    // 11번 임플란트 이미지
}

void Maxlayout::slotD_12(QPixmap* teeth12)
{
    showTeeth = teeth12;    // 12번 임플란트 이미지
}

void Maxlayout::slotD_13(QPixmap* teeth13)
{
    showTeeth = teeth13;    // 13번 임플란트 이미지
}

void Maxlayout::slotD_14(QPixmap* teeth14)
{
    showTeeth = teeth14;    // 14번 임플란트 이미지
}

void Maxlayout::slotD_15(QPixmap* teeth15)
{
    showTeeth = teeth15;    // 15번 임플란트 이미지
}

void Maxlayout::slotD_16(QPixmap* teeth16)
{
    showTeeth = teeth16;    // 16번 임플란트 이미지
}

void Maxlayout::slotD_17(QPixmap* teeth17)
{
    showTeeth = teeth17;    // 17번 임플란트 이미지
}

/* Top Right Teeth */
void Maxlayout::slotD_21(QPixmap* teeth21)
{
    showTeeth = teeth21;    // 21번 임플란트 이미지
}
void Maxlayout::slotD_22(QPixmap* teeth22)
{
    showTeeth = teeth22;    // 22번 임플란트 이미지
}
void Maxlayout::slotD_23(QPixmap* teeth23)
{
    showTeeth = teeth23;    // 23번 임플란트 이미지
}
void Maxlayout::slotD_24(QPixmap* teeth24)
{
    showTeeth = teeth24;    // 24번 임플란트 이미지
}
void Maxlayout::slotD_25(QPixmap* teeth25)
{
    showTeeth = teeth25;    // 25번 임플란트 이미지
}
void Maxlayout::slotD_26(QPixmap* teeth26)
{
    showTeeth = teeth26;    // 26번 임플란트 이미지
}
void Maxlayout::slotD_27(QPixmap* teeth27)
{
    showTeeth = teeth27;    // 27번 임플란트 이미지
}

/* Top Right Teeth */
void Maxlayout::slotD_31(QPixmap* teeth31)
{
    showTeeth = teeth31;    // 31번 임플란트 이미지
}
void Maxlayout::slotD_32(QPixmap* teeth32)
{
    showTeeth = teeth32;    // 32번 임플란트 이미지
}
void Maxlayout::slotD_33(QPixmap* teeth33)
{
    showTeeth = teeth33;    // 33번 임플란트 이미지
}
void Maxlayout::slotD_34(QPixmap* teeth34)
{
    showTeeth = teeth34;    // 34번 임플란트 이미지
}
void Maxlayout::slotD_35(QPixmap* teeth35)
{
    showTeeth = teeth35;    // 35번 임플란트 이미지
}
void Maxlayout::slotD_36(QPixmap* teeth36)
{
    showTeeth = teeth36;    // 36번 임플란트 이미지
}
void Maxlayout::slotD_37(QPixmap* teeth37)
{
    showTeeth = teeth37;    // 37번 임플란트 이미지
}

/* Bottom Left Teeth */
void Maxlayout::slotD_41(QPixmap* teeth41)
{
    showTeeth = teeth41;    // 41번 임플란트 이미지
}
void Maxlayout::slotD_42(QPixmap* teeth42)
{
    showTeeth = teeth42;    // 42번 임플란트 이미지
}
void Maxlayout::slotD_43(QPixmap* teeth43)
{
    showTeeth = teeth43;    // 43번 임플란트 이미지
}
void Maxlayout::slotD_44(QPixmap* teeth44)
{
    showTeeth = teeth44;    // 44번 임플란트 이미지
}
void Maxlayout::slotD_45(QPixmap* teeth45)
{
    showTeeth = teeth45;    // 45번 임플란트 이미지
}
void Maxlayout::slotD_46(QPixmap* teeth46)
{
    showTeeth = teeth46;    // 46번 임플란트 이미지
}
void Maxlayout::slotD_47(QPixmap* teeth47)
{
    showTeeth = teeth47;    // 47번 임플란트 이미지
}

