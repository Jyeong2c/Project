#ifndef MAXLAYOUT_H
#define MAXLAYOUT_H

#include <QWidget>

class QHBoxLayout;
class QGraphicsView;
class QGraphicsScene;
class QPushButton;

class View;
class Scene;

class Maxlayout : public QWidget
{
    Q_OBJECT

public:
    explicit Maxlayout(QWidget *parent = nullptr);

    /* 멤버 변수 */
    View *maxNewGrid;   // QGraphicsView
    Scene *maxNewSc;    // QGraphicsScene
    QPushButton *viewQuit;  // 종료 x버튼
    QPointF clickPoint; // 마우스 가운데 버튼 클릭 좌표 위치

    void setNewSc(Scene*);
    void setNewGrid(View*);


public slots:
    /* Top Left Teeth */
    void slotD_11(QPixmap* teeth11);    // 11번 임플란트 이미지
    void slotD_12(QPixmap* teeth12);    // 12번 임플란트 이미지
    void slotD_13(QPixmap* teeth13);    // 13번 임플란트 이미지
    void slotD_14(QPixmap* teeth14);    // 14번 임플란트 이미지
    void slotD_15(QPixmap* teeth15);    // 15번 임플란트 이미지
    void slotD_16(QPixmap* teeth16);    // 16번 임플란트 이미지
    void slotD_17(QPixmap* teeth17);    // 17번 임플란트 이미지

    /* Top Right Teeth */
    void slotD_21(QPixmap* teeth21);    // 21번 임플란트 이미지
    void slotD_22(QPixmap* teeth22);    // 22번 임플란트 이미지
    void slotD_23(QPixmap* teeth23);    // 23번 임플란트 이미지
    void slotD_24(QPixmap* teeth24);    // 24번 임플란트 이미지
    void slotD_25(QPixmap* teeth25);    // 25번 임플란트 이미지
    void slotD_26(QPixmap* teeth26);    // 26번 임플란트 이미지
    void slotD_27(QPixmap* teeth27);    // 27번 임플란트 이미지

    /* Bottom Right Teeth */
    void slotD_31(QPixmap* teeth31);    // 31번 임플란트 이미지
    void slotD_32(QPixmap* teeth32);    // 32번 임플란트 이미지
    void slotD_33(QPixmap* teeth33);    // 33번 임플란트 이미지
    void slotD_34(QPixmap* teeth34);    // 34번 임플란트 이미지
    void slotD_35(QPixmap* teeth35);    // 35번 임플란트 이미지
    void slotD_36(QPixmap* teeth36);    // 36번 임플란트 이미지
    void slotD_37(QPixmap* teeth37);    // 37번 임플란트 이미지

    /* Bottom Left Teeth */
    void slotD_41(QPixmap* teeth41);    // 41번 임플란트 이미지
    void slotD_42(QPixmap* teeth42);    // 42번 임플란트 이미지
    void slotD_43(QPixmap* teeth43);    // 43번 임플란트 이미지
    void slotD_44(QPixmap* teeth44);    // 44번 임플란트 이미지
    void slotD_45(QPixmap* teeth45);    // 45번 임플란트 이미지
    void slotD_46(QPixmap* teeth46);    // 46번 임플란트 이미지
    void slotD_47(QPixmap* teeth47);    // 47번 임플란트 이미지


protected:
    void resizeEvent(QResizeEvent * event) override;    // Maxlayout의 크기가 변경될 때 실행하는 함수 override
    void mousePressEvent(QMouseEvent *event) override;  // 마우스를 클릭했을 때 실행하는 함수 override


private:
    /* 멤버 변수 */
    QWidget *maxWidget;
    QHBoxLayout *maxLayBox;
    QPixmap* showTeeth; // 인자로 받은 치아 이미지를 담는 변수


signals:
    void sig_clicked(QPointF);  // 현재 좌표 위치를 시그널로 전달
};

#endif // MAXLAYOUT_H
