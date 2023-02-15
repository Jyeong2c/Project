#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QHash>

class TeethForm;
class QMouseEvent;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    enum Shape {Cursor, Implant, Line, Rect, Ellipse,
                Path, Arrow, Triangle, Text, Length, Angle};

    void setCurrentShape(const Shape &value);
    void addImplantItem(QPointF stPos, QPointF edPos, QPointF mPos);    // 임플란트를 출력하는 함수 (첫번째 좌표, 두번째 좌표, 세번째 좌표)


/* EUN JI */
// ----------------------------------------

//    Shape getCurrentShape() const;

    QColor getCurrentColor() const;
    void setCurrentColor(const QColor& color);

    qreal rotation() const;
    void setRotation(qreal angle);

    void setText(QString string);

// ----------------------------------------


    // List
    QList<QGraphicsPathItem*> m_pathList;
    QList<QGraphicsItem*> m_allitems;
    QList<QGraphicsItem*> m_implantItems;
    QList<QGraphicsItem*> m_point;

    bool ts = false;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // 마우스를 클릭했을 때 실행하는 함수 override
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;  // 마우스를 누르고 움직였을 때 실행하는 함수 override
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;   // 마우스를 눌렀다가 뗐을 때 실행하는 함수 override


/* EUN JI */
// ---------------------------------------------------
    void keyPressEvent(QKeyEvent *event) override;

    void addLineItem(QPointF stPos, QPointF edPos);
    void addRectItem(QPointF stPos, QPointF edPos);
    void addEllipseItem(QPointF stPos, QPointF edPos);
    void addImageItem(QPointF stPos, QPointF edPos);
    void addTriangleItem(QPointF stPos, QPointF midPos, QPointF edPos);
    void addArrowItem(QPointF stPos, QPointF edPos);
    void addTextItem(QPointF stPos);
// ---------------------------------------------------


/* JAE HY */
// ---------------------------------------------------
    void addLengthItem(QPointF stPos, QPointF edPos);                       /*길이 측정 아이템 출력후 moveable 지정*/
    void addAngleItem(QPointF stPos,QPointF mdPos, QPointF edPos);          /*각도 측정 아이템 출력후 moveable 지정*/
// ---------------------------------------------------

private:
    Shape m_currentShape;
    QGraphicsItem* m_currentItem;

    bool m_isClicked;
    QPointF m_prevPos;  // 이전 좌표값
    QPointF m_latePos;  // 이후 좌표값
    QPointF m_sPos;      // 세컨 좌표값

    int m_imCount;


    QPixmap* showTeeth;         // 슬롯으로 받은 치아 이미지를 담는 변수
    QString ipH;



/* EUN JI*/
//------------------------------------------------------------------
    bool drawing;
    bool drawEllipse = false;

    int m_triClicked;

    //PathItem
    QGraphicsPathItem* m_pathItem;

    QColor m_currentColor;
    QPointF m_startPos;
    QPointF m_midPos;

    qreal paintSize;        // 선의 크기를 기억하는 변수
    qreal rotateAngle;

    QString text;

//------------------------------------------------------------------


/* JAE HY*/
//------------------------------------------------------------------
    int lengthFstX, lengthFstY; //길이 측정 첫번째 x, y 좌표
    int lengthLastX, lengthLastY; //길이 측정 마지막 x, y 좌표

    /*각도 측정용 1번째, 2번째 좌표 설정*/
    QPointF m_fstAnglePos;
    QPointF m_sedAnglePos;

    int pointCount = 0;         //각도 측정 카운터

    int fstPosX, fstPosY;       //각도 측정 첫번째 x,y 좌표
    int sedPosX, sedPosY;       //각도 측정 두번째 x,y 좌표
    int trdPosX, trdPosY;       //각도 측정 세번째 x,y 좌표

    int imageWidth;             // Scene에 할당된 이미지 width 변수
    int imageHeight;            // Scene에 할당된 이미지 height 변수
    double imagePixel;          // 픽셀당 길이값 변수

//    MainWindow *mainWindow;
//------------------------------------------------------------------


public slots:
    void clearItems();
    void implantClearItems();

    /* Top Left Teeth */
    void slot_11(QPixmap* teeth11);
    void slot_12(QPixmap* teeth12);
    void slot_13(QPixmap* teeth13);
    void slot_14(QPixmap* teeth14);
    void slot_15(QPixmap* teeth15);
    void slot_16(QPixmap* teeth16);
    void slot_17(QPixmap* teeth17);

     /* Top Right Teeth */
    void slot_21(QPixmap* teeth21);
    void slot_22(QPixmap* teeth22);
    void slot_23(QPixmap* teeth23);
    void slot_24(QPixmap* teeth24);
    void slot_25(QPixmap* teeth25);
    void slot_26(QPixmap* teeth26);
    void slot_27(QPixmap* teeth27);

    /* Bottom Right Teeth */
    void slot_31(QPixmap* teeth31);
    void slot_32(QPixmap* teeth32);
    void slot_33(QPixmap* teeth33);
    void slot_34(QPixmap* teeth34);
    void slot_35(QPixmap* teeth35);
    void slot_36(QPixmap* teeth36);
    void slot_37(QPixmap* teeth37);

    /* Bottom Left Teeth */
    void slot_41(QPixmap* teeth41);
    void slot_42(QPixmap* teeth42);
    void slot_43(QPixmap* teeth43);
    void slot_44(QPixmap* teeth44);
    void slot_45(QPixmap* teeth45);
    void slot_46(QPixmap* teeth46);
    void slot_47(QPixmap* teeth47);


/* EUN JI*/
//------------------------------------------------------------------
    void setPaintSize(qreal);
    void deleteItems();
    void rotate();
//------------------------------------------------------------------


/* JAE HY */
//------------------------------------------------------------------
    /*Scene클래스 내에 2점의 좌표를 받는 슬롯 함수*/
    void reFirstOrtho(int _x, int _y);
    void reLastOrtho(int _x, int _y);

    /*Scene클래스내에 3점의 좌표를 받는 슬롯 함수*/
    void reFirstAnglePoint(int _x, int _y);
    void reSecondAnglePoint(int _x, int _y);
    void reThirdAnglePoint(int _x, int _y);

    void reImageWHP(int _width, int _height, double _pixels);       // 이미지의 정보를 받아오는 함수
//------------------------------------------------------------------

signals:

/* JAE HY */
//------------------------------------------------------------------
    void sendFirstOrtho(int _x, int _y);
    void sendLastOrtho(int _x, int _y);

    /*각도 측정 관련 시그널*/
    void firstAnglePoint(int _x, int _y);
    void secondAnglePoint(int _x, int _y);
    void thirdAnglePoint(int _x, int _y);

    void sendLengthMeasure(double _length);
    void sendAngleMeasure(double _angle);
//------------------------------------------------------------------

};

#endif // SCENE_H
