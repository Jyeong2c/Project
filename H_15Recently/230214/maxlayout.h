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

//    QGraphicsView *maxNewGrid;
    View *maxNewGrid;


//    QGraphicsScene *maxNewSc;
    Scene *maxNewSc;

    QPushButton *viewQuit;

    QPoint DPointLeft;
    QPointF clickPoint;

    void setNewSc(Scene*);
//    void setNewSc(QGraphicsScene*);
    void setNewGrid(View*);

public slots:

    /* Top Left Teeth */
    void slotD_11(QPixmap* teeth11);
    void slotD_12(QPixmap* teeth12);
    void slotD_13(QPixmap* teeth13);
    void slotD_14(QPixmap* teeth14);
    void slotD_15(QPixmap* teeth15);
    void slotD_16(QPixmap* teeth16);
    void slotD_17(QPixmap* teeth17);

    /* Top Right Teeth */
    void slotD_21(QPixmap* teeth21);
    void slotD_22(QPixmap* teeth22);
    void slotD_23(QPixmap* teeth23);
    void slotD_24(QPixmap* teeth24);
    void slotD_25(QPixmap* teeth25);
    void slotD_26(QPixmap* teeth26);
    void slotD_27(QPixmap* teeth27);

    /* Bottom Right Teeth */
    void slotD_31(QPixmap* teeth31);
    void slotD_32(QPixmap* teeth32);
    void slotD_33(QPixmap* teeth33);
    void slotD_34(QPixmap* teeth34);
    void slotD_35(QPixmap* teeth35);
    void slotD_36(QPixmap* teeth36);
    void slotD_37(QPixmap* teeth37);

    /* Bottom Left Teeth */
    void slotD_41(QPixmap* teeth41);
    void slotD_42(QPixmap* teeth42);
    void slotD_43(QPixmap* teeth43);
    void slotD_44(QPixmap* teeth44);
    void slotD_45(QPixmap* teeth45);
    void slotD_46(QPixmap* teeth46);
    void slotD_47(QPixmap* teeth47);



protected:
    void resizeEvent(QResizeEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    QWidget *maxWidget;
    QHBoxLayout *maxLayBox;

    QPixmap* showTeeth;         // 슬롯으로 받은 치아 이미지를 담는 변수


signals:
    void sig_clicked(QPointF);
    void sig_leftDClicked(QPointF);

};



#endif // MAXLAYOUT_H
