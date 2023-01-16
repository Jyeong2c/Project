#include "layout.h"

#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>

#include <QGraphicsLineItem>
#include <QWheelEvent>

#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)
Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();

    image = new QImage();
    //    imgSize->load("C:/Users/KOSA/Desktop/HTML Canvas/HTML Canvas/orthopan.jpg");
    image->load("./panorama1.png");
    //    if(QColorConstants::Gray == true){
    //        QImgae *temp  = new QImgae(data,width,height,QImage::Format_RGB888);
    //    }

    QPixmap buf = QPixmap::fromImage(*image);
    buf = buf.scaled(image->width(), image->height());

    scene = new QGraphicsScene;
    scene->addPixmap(buf);
    grid1->setScene(scene);
    //grid1->setBackgroundBrush(Qt::black);

    //생성자에서 블러오는게 아닌 버튼을 클릭시 이벤트 발생되어야 함
    //if문으로 생성한다면 어떻게 될까
    QPen mPen; //펜의 변수 선언
    mPen.setWidth(2); //펜의 굵기 5
    mPen.setColor(Qt::red); //펜의 색상 노란색

    //    /* 선 만들기 */
    //    lineItem = new QGraphicsLineItem(); //QLineItem 변수 지정(선 만들기)
    //    lineItem->setLine(0, 0, 20, 0);        //선 의 좌표 설정
    //    //lineItem->setFlag(QGraphicsItem::ItemIsFocusable);  //focus 설정, 고정
    //    lineItem->setFlag(QGraphicsItem::ItemIsMovable);    //고정해제
    //    lineItem->setFlag(QGraphicsItem::ItemIsSelectable); //선택 레이어 표출
    //    lineItem->setFocus();       //포커싱
    //    lineItem->setPen(mPen); //선언된 펜 변수를 lineItem에 적용
    //    scene->addItem(lineItem);   //lineItem 추가
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
void Layout::brightUp()
{
    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
            const QRgb rgb = image->pixel(x, y);
            if(rgb < 255){
                const double r =LIMIT_UBYTE(qRed(rgb)   + 5);
                const double g =LIMIT_UBYTE(qGreen(rgb) + 5);
                const double b =LIMIT_UBYTE(qBlue(rgb)  + 5);

                image->setPixelColor(x, y,QColor(r,g,b));
            }
        }
    scene = new QGraphicsScene;

    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);
}

void Layout::brightnessLow()
{
    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
            const QRgb rgb = image->pixel(x, y);
            if(rgb >0 ){
                const double r =LIMIT_UBYTE(qRed(rgb)   - 5);
                const double g =LIMIT_UBYTE(qGreen(rgb) - 5);
                const double b =LIMIT_UBYTE(qBlue(rgb)  - 5);

                image->setPixelColor(x, y,QColor(r,g,b));
                //            image->invertPixels(255 - );
            }
        }
    scene = new QGraphicsScene;

    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);
}

void Layout::inverstion()
{
    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
            const QRgb rgb = image->pixel(x, y);
            const double r =LIMIT_UBYTE(255- qRed(rgb));
            const double g =LIMIT_UBYTE(255 - qGreen(rgb));
            const double b =LIMIT_UBYTE(255 - qBlue(rgb));

            image->setPixelColor(x, y,QColor(r,g,b));
            //            image->invertPixels(255 - );
        }
    scene = new QGraphicsScene;

    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);
}

void Layout::proTractor()
{
    //    QFileDialog dlg;
    //    QString filePath = dlg.getOpenFileName(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");

    //    QImage *imgSize = new QImage(filePath);
    //    QPixmap buf = QPixmap::fromImage(*imgSize);
    //    buf = buf.scaled(imgSize->width(), imgSize->height());

    //    scene = new Scene;
    //    scene->addPixmap(buf);

    //    QGraphicsView *view = new QGraphicsView(this);
    //    view->setScene(scene);
    //    ui->verticalLayout->addWidget(view);
    scene = new QGraphicsScene(this);
    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);

    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    QPen mPen; //펜의 변수 선언
    mPen.setWidth(5); //펜의 굵기 5
    mPen.setColor(Qt::red); //펜의 색상 붉은색

    /* 선 만들기 */
    lineItem = new QGraphicsLineItem(); //QLineItem 변수 지정(선 만들기)
    lineItem->setLine(0, 0, 20, 0);        //선 의 좌표 설정
    //lineItem->setFlag(QGraphicsItem::ItemIsFocusable);  //focus 설정, 고정
    lineItem->setFlag(QGraphicsItem::ItemIsMovable);    //고정해제
    lineItem->setFlag(QGraphicsItem::ItemIsSelectable); //선택 레이어 표출
    lineItem->setFocus();       //포커싱
    lineItem->setPen(mPen); //선언된 펜 변수를 lineItem에 적용
    scene->addItem(lineItem);   //lineItem 추가
}

/*휠 이벤트를 부여하여 휠을 위/아래로 돌렸을 때 기준점, 기준선에 맞춰 벌어진 각도를 계산*/
void Layout::wheelEvent(QWheelEvent *ev)
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    //crash가 났음..
    /*주의 사항은 후치연산(rotate++)을 하는 경우 휠을 반대로 동작할 때 한번 딜레이되어 이상한 결과를 만듦*/
    if(ev->angleDelta().y() > 0) // up Wheel
        lineItem->setRotation(--rotate);

    else if(ev->angleDelta().y() < 0) //down Wheel
        lineItem->setRotation(++rotate);

    /*휠의 동작에 따른 각도 움직임을 라인에디터에 출력*/
    if(rotate < 0){
        //ui->rotateEdit->setText(QString("%1").arg(-1 * rotate));
        qDebug() << -1 * rotate;
        /*각도 측정시 360이상은 측정할 필요가 없으므로 측정값이 360인 경우 rotate를 0으로 초기화*/
        if(rotate == 360)
            rotate = 0;
    }
    else{
        //ui->rotateEdit->setText(QString("%1").arg(rotate));
        qDebug() << rotate;
        if(rotate == 360)
            rotate = 0;
    }

}


/*메인 윈도우의 커넥트 SIGNAL을 받기위한 SLOT*/
void Layout::ruler(){
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    m_nbMousePressed = false;
}

/*길이 측정을 위한 라인 마우스 이벤트*/
void Layout::mousePressEvent(QMouseEvent *ev){
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    m_nbMousePressed = true;
//    m_line.setP1(ev->pos());
//    m_line.setP2(ev->pos());
    origPoint = ev->pos();
}
void Layout::mouseMoveEvent(QMouseEvent *ev){
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

    scene = new QGraphicsScene(this);
    QPixmap buf = QPixmap::fromImage(*image);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    scene->addPixmap(buf);

    if(ev->type() == QEvent::MouseMove)
    {
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem;
            //grid1 의 scene에 그리기로
            scene->addItem(itemToDraw);
            itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0,0,
                            ev->pos().x() - origPoint.x(),
                            ev->pos().y() - origPoint.y());
    }
    Layout::mousePressEvent(ev);
    update();
}
void Layout::mouseReleaseEvent(QMouseEvent* ev){
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    itemToDraw = 0;
    m_nbMousePressed = false;
    Layout::mouseReleaseEvent(ev);
    update();
}
//void Layout::paintEvent(QPaintEvent* /*e*/){
//    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
//    static bool wasPressed = false;

//    QGraphicsLineItem *itemToDraw;
//    QPointF origPoint;

//    //QPainter 에서는 QGraphicsScene을 받을 수 없다.
//    (scene);

//    if(m_nbMousePressed)
//    {
//        painter.drawPixmap(0, 0, buff);
//        painter.drawLine(m_line);
//        wasPressed = true;
//    }else if(wasPressed){
//        QPainter PixmapPainter(&buff);
//        QPen pen(Qt::green);
//        PixmapPainter.setPen(pen);
//        PixmapPainter.drawLine(m_line);

//        painter.drawPixmap(0, 0, buff);
//        wasPressed = false;
//    }

//}


//QImage *image = new QImage(ui->listWidget->currentItem()->statusTip());

//for(int x = 0; x < image->width(); ++x)
//        for(int y = 0 ; y < image->height(); ++y) {
//            const QRgb rgb = image->pixel(x, y);
//            const double r = LIMIT_UBYTE(qRed(rgb) + value);
//            const double g = LIMIT_UBYTE(qGreen(rgb) + value);
//            const double b = LIMIT_UBYTE(qBlue(rgb) + value);
//            image->setPixelColor(x, y,
//                                 QColor(
//                                     r,
//                                     g,
//                                     b));
//        }

//QPixmap buf = QPixmap::fromImage(image_Histogram);
//imageView->setScene(imageView->graphicsScene);
//imageView->graphicsScene->addPixmap(buf.scaled(imageView->width(), imageView->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));


