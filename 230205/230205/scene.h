#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class TeethForm;
class QMouseEvent;


class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    bool _pan;
    int _panStartX, _panStartY;

    enum Shape {Cursor, Implant};

    void setCurrentShape(const Shape &value);


//    QPixmap *showTeeth;
//    QGraphicsPixmapItem *toothItem;


    void addImplantItem(QPointF stPos, QPointF edPos);          // 마우스 위치에 임플란트 이미지를 출력하는 함수
    //void addImplantItem(QPointF stPos, QPointF edPos, QPoint mPos);



    QGraphicsPixmapItem *pixmapItem;


    QList<QGraphicsPathItem*> m_pathList;
    QList<QGraphicsItem*> m_allitems;
    QList<QGraphicsItem*> m_implantItems;
    QList<QGraphicsItem*> m_point;




protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);              // 왼쪽 마우스를 눌렀을 때 좌표 위치
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);            // 마우스를 뗐을 때
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
//    TeethForm *teeth;




    Shape m_currentShape;
    QGraphicsItem* m_currentItem;

    bool m_isClicked;
    QPointF m_prevPos;  // 이전 좌표값
    QPointF m_latePos;  // 이후 좌표값

    QPoint m_sPos;      // 세컨 좌표값

    int m_imCount;






public slots:
    void clearItems();


    //    /* Top Left Teeth */
    void slot_11();
    void slot_12();
    //    void slot_11(QPixmap*);
    //    void slot_12(QPixmap*);
    //    void slot_13(QPixmap*);
    //    void slot_14(QPixmap*);
    //    void slot_15(QPixmap*);
    //    void slot_16(QPixmap*);
    //    void slot_17(QPixmap*);

    //    /* Top Right Teeth */
    //    void slot_21(QPixmap*);
    //    void slot_22(QPixmap*);
    //    void slot_23(QPixmap*);
    //    void slot_24(QPixmap*);
    //    void slot_25(QPixmap*);
    //    void slot_26(QPixmap*);
    //    void slot_27(QPixmap*);

    //    /* Bottom Right Teeth */
    //    void slot_31(QPixmap*);
    //    void slot_32(QPixmap*);
    //    void slot_33(QPixmap*);
    //    void slot_34(QPixmap*);
    //    void slot_35(QPixmap*);
    //    void slot_36(QPixmap*);
    //    void slot_37(QPixmap*);

    //    /* Bottom Left Teeth */
    //    void slot_41(QPixmap*);
    //    void slot_42(QPixmap*);
    //    void slot_43(QPixmap*);
    //    void slot_44(QPixmap*);
    //    void slot_45(QPixmap*);
    //    void slot_46(QPixmap*);
    //    void slot_47(QPixmap*);




signals:
//    void sig_move(QGraphicsSceneMouseEvent *);

};

#endif // SCENE_H
