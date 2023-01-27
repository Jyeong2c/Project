#include "layout.h"
#include "scene.h"

#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QPixelFormat>


#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

Layout::Layout(QWidget *parent)
    : QWidget{parent}
{
    gridLayout();

    m_image = new QImage();

    m_image->load("C:/Image/Images.jpg");
    //        image->load("C:/Users/KOSA/Desktop/Qt/ㅇㅁㅈ.jpg");
    //        image->load("C:/Users/KOSA/Desktop/Qt/image.bmp");

    //        image->convertTo(QImage::Format_Grayscale8);

    //    if(QColorConstants::Gray == true){
    //        QImgae *temp  = new QImgae(data,width,height,QImage::Format_RGB888);
    //    }

    m_copyImage = new QImage();
    m_copyImage = m_image;

    QPixmap buf = QPixmap::fromImage(*m_image);
    buf = buf.scaled(m_image->width(), m_image->height());


    scene = new Scene();
    scene->addPixmap(buf);
    grid1->setScene(scene);

    //grid1->setBackgroundBrush(Qt::black);
}

/* 2 X 2 Grid */
void Layout::gridLayout()
{
    //QWidget *window = new QWidget;

    grid1 = new QGraphicsView;
    grid2 = new QGraphicsView;
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
    //    intensity+=5;
    //    //    int tmp;

    //    QImage image_to_display =  QImage(*image);

    //    for(int x = 0; x < image_to_display.width(); ++x){
    //        for(int y = 0 ; y < image_to_display.height(); ++y){

    //            const QRgb rgb = image->pixel(x, y);
    //            const double r =LIMIT_UBYTE(qRed(rgb)   + intensity);
    //            const double g =LIMIT_UBYTE(qGreen(rgb) + intensity);
    //            const double b =LIMIT_UBYTE(qBlue(rgb)  + intensity);

    //            image_to_display.setPixelColor(x, y,QColor(r,g,b));
    //        }
    //    }
    //    scene = new QGraphicsScene;
    //    //    *image = image_to_display;
    //    QPixmap buf = QPixmap::fromImage(image_to_display);
    //    scene->addPixmap(buf.scaled(image_to_display.width(),image_to_display.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //    grid1->setScene(scene);
}

void Layout::brightnessLow()
{
    //    intensity-=5;

    //    QImage image_to_display =  QImage(*image);

    //    for(int x = 0; x < image_to_display.width(); ++x){
    //        for(int y = 0 ; y < image_to_display.height(); ++y) {
    //            const QRgb rgb = image->pixel(x, y);

    //            const double r =LIMIT_UBYTE(qRed(rgb)   + intensity);
    //            const double g =LIMIT_UBYTE(qGreen(rgb) + intensity);
    //            const double b =LIMIT_UBYTE(qBlue(rgb)  + intensity);

    //            image_to_display.setPixelColor(x, y,QColor(r,g,b));
    //        }
    //    }
    //    scene = new QGraphicsScene;
    //    //    *image = image_to_display;
    //    QPixmap buf = QPixmap::fromImage(image_to_display);
    //    scene->addPixmap(buf.scaled(image_to_display.width(),image_to_display.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //    grid1->setScene(scene);
    //    //    scene->addPixmap(buf);
}

void Layout::inverstion()
{
    m_copyImage->invertPixels();
    //    m_image->invertPixels();
    //    phaseOneImage->invertPixels();

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(*m_image);
    scene->addPixmap(buf.scaled(m_image->width(),m_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));;
    grid1->setScene(scene);
    scene->addPixmap(buf);
}

void Layout::verticalFlip()
{
    m_copyImage->mirror(true,false);
    //    m_image->mirror(true,false);
    //        phaseOneImage->mirror(true,false);

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(*m_image);
    scene->addPixmap(buf.scaled(m_image->width(),m_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    grid1->setScene(scene);
    scene->addPixmap(buf);
}
void Layout::horizontalFlip()
{
    m_copyImage->mirror(false,true);
    //    m_image->mirror(false,true);

    //    //    phaseOneImage->mirror(false,true);

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(*m_image);
    scene->addPixmap(buf.scaled(m_image->width(),m_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    grid1->setScene(scene);



    //    grid1->rotate(180);
    //    grid1->setGraphicsEffect(PixmapPadMode::PadToTransparentBorder);
}

void Layout::zoomIn()
{
    grid1->scale(1.25,1.25);
}

void Layout::zoomOut()
{
    grid1->scale(0.8, 0.8);
}

void Layout::leftRotate()
{
    grid1->rotate(-90);
}

void Layout::rightRotate()
{
    grid1->rotate(90);
}

void Layout::brightnessDownOpenCV()
{
    intensity -=10;
    QImage image_to_display =  QImage(*m_image);
    image_to_display = image_to_display.convertToFormat(QImage::Format_RGB888);
    //    cv::Mat mat = cv::Mat(image_to_display.height(),
    //                          image_to_display.width(),
    //                          CV_8UC3,
    //                          image_to_display.bits(),
    //                          image_to_display.bytesPerLine());


    //    Mat brightnessUp;
    //    mat.convertTo(brightnessUp, -1, 1, intensity);
    //    //     Mat brightnessLow = mat - 100;

    //    QImage outImg(brightnessUp.data,
    //                  brightnessUp.cols,
    //                  brightnessUp.rows,
    //                  brightnessUp.step,
    //                  QImage::Format_RGB888);

    //    scene = new QGraphicsScene;
    //    QPixmap buf = QPixmap::fromImage(outImg);
    //    scene->addPixmap(buf);
    //    grid1->setScene(scene);
    //    QImage image_to_display =  QImage(*image);
    //    *m_image = m_image->convertToFormat(QImage::Format_RGB888);
    cv::Mat mat = cv::Mat(image_to_display.height(),
                          image_to_display.width(),
                          CV_8UC3,
                          image_to_display.bits(),
                          image_to_display.bytesPerLine());


    Mat brightnessDown;
    mat.convertTo(brightnessDown, -1, 1, intensity);
    //     Mat brightnessLow = mat - 100;

    QImage outImg(brightnessDown.data,
                  brightnessDown.cols,
                  brightnessDown.rows,
                  brightnessDown.step,
                  QImage::Format_RGB888);

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(outImg);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    //    *m_copyImage = buf.toImage().convertToFormat(QImage::Format_RGB888);


}
void Layout::brightSlider(int intensity)
{
    intensity +=10;
    *m_image = m_image->convertToFormat(QImage::Format_RGB888);
    cv::Mat mat = cv::Mat(m_image->height(),
                          m_image->width(),
                          CV_8UC3,
                          m_image->bits(),
                          m_image->bytesPerLine());


    Mat brightness;
    mat.convertTo(brightness, -1, 1, intensity);
    //     Mat brightnessLow = mat - 100;

    QImage outImg(brightness.data,
                  brightness.cols,
                  brightness.rows,
                  brightness.step,
                  QImage::Format_RGB888);

    scene = new Scene();
    //    image = &outImg;
    QPixmap buf = QPixmap::fromImage(outImg);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    //    *m_copyImage = buf.toImage().convertToFormat(QImage::Format_RGB888);

}

void Layout::brightnessUpOpenCV()
{
    intensity +=10;
    //    QImage image_to_display =  QImage(*image);
    //    image_to_display = image_to_display.convertToFormat(QImage::Format_RGB888);
    //    cv::Mat mat = cv::Mat(image_to_display.height(),
    //                          image_to_display.width(),
    //                          CV_8UC3,
    //                          image_to_display.bits(),
    //                          image_to_display.bytesPerLine());


    //    Mat brightnessUp;
    //    mat.convertTo(brightnessUp, -1, 1, intensity);
    //    //     Mat brightnessLow = mat - 100;

    //    QImage outImg(brightnessUp.data,
    //                  brightnessUp.cols,
    //                  brightnessUp.rows,
    //                  brightnessUp.step,
    //                  QImage::Format_RGB888);

    //    scene = new QGraphicsScene;
    //    QPixmap buf = QPixmap::fromImage(outImg);
    //    scene->addPixmap(buf);
    //    grid1->setScene(scene);
    *m_image = m_image->convertToFormat(QImage::Format_RGB888);
    cv::Mat mat = cv::Mat(m_image->height(),
                          m_image->width(),
                          CV_8UC3,
                          m_image->bits(),
                          m_image->bytesPerLine());


    Mat brightnessUp;
    mat.convertTo(brightnessUp, -1, 1, intensity);
    //     Mat brightnessLow = mat - 100;

    QImage outImg(brightnessUp.data,
                  brightnessUp.cols,
                  brightnessUp.rows,
                  brightnessUp.step,
                  QImage::Format_RGB888);

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(outImg);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    //    *m_copyImage = buf.toImage().convertToFormat(QImage::Format_RGB888);
    //    image = &outImg;
}

void Layout::his()
{
    //평활화 -> 그레이 스케일 만  가능
    *m_image = m_image->convertToFormat(QImage::Format_Grayscale8);
    //    image_to_display = image_to_display.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat mat = cv::Mat(m_image->height(),
                          m_image->width(),
                          CV_8UC1,
                          m_image->bits(),
                          m_image->bytesPerLine());


    Mat dst;
    equalizeHist(mat, dst);

    ////    alpha = 1.0
    ////    func = (1+alpha) * src - (alpha * 128)
    ////    dst = np.clip(func, 0, 255).astype(np.uint8)
    ////    normalize(mat, his);
    //    //mat.convertTo(brightnessUp, -1, 1, intensity);
    //    normalize(mat, None, 0, 255, NORM_MINMAX);
    ////    mat = equalizeHist(src)
    //    //     Mat brightnessLow = mat - 100;

    QImage outImg(dst.data,
                  dst.cols,
                  dst.rows,
                  dst.step,
                  QImage::Format_Grayscale8);

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(outImg);
    scene->addPixmap(buf);
    grid1->setScene(scene);
    //    image = &outImg;
}


void Layout::sharpen()
{
    inten +=1;
    cnt++;
    qDebug() << cnt;

    if(cnt > 5){
        cnt = 0;
        inten = 0;
        m_image = new QImage;
        scene = new Scene();
        qDebug()<<__LINE__;
        QPixmap buf = QPixmap::fromImage(*m_image);
        scene->addPixmap(buf);
        qDebug()<<__LINE__;
        grid1->setScene(scene);

    }else{
        qDebug()<<__LINE__;

        QImage image_to_display =  QImage(*m_image).convertToFormat(QImage::Format_RGB888);

        //    *image->convertToFormat(QImage::Format_RGB888);
        //    image_to_display = image_to_display.convertToFormat(QImage::Format_RGB888);
        //    image_to_display = image_to_display.convertToFormat(QImage::Format_Grayscale8);
        cv::Mat mat = cv::Mat(image_to_display.height(),
                              image_to_display.width(),
                              CV_8UC3,
                              image_to_display.bits(),
                              image_to_display.bytesPerLine());

        //1. 입력이미지 2.출력이미지  3.커널의 크기  4.가우스 커널 표준편차를 나타내는 double형 변수
        Mat smoothed;
        GaussianBlur(mat, smoothed,Size(5,5),0);
        Mat sharpenImage = mat + (mat- smoothed) * inten;


        //    cv::Mat tmp;
        //          cv::blur(mat, tmp, cv::Size(8, 8));
        //          mat = tmp;

        ////    cvtColor(mat, tmp,)

        QImage outImg(sharpenImage.data,
                      sharpenImage.cols,
                      sharpenImage.rows,
                      sharpenImage.step,
                      QImage::Format_RGB888);
        //    Mat img_blur;
        //    blur(mat, img_blur, Size(5,5));

        //    Mat img_gap = mat - img_blur;
        //    Mat img_sharp = mat + img_gap * inten;

        //    QImage image_blurred(
        //                          img_sharp.data,
        //                          img_sharp.cols,
        //                          img_sharp.rows,
        //                          img_sharp.step,
        //                          QImage::Format_RGB888);


        scene = new Scene();
        QPixmap buf = QPixmap::fromImage(outImg);
        scene->addPixmap(buf);
        grid1->setScene(scene);

        *m_copyImage = buf.toImage().convertToFormat(QImage::Format_RGB888);
        //    *m_image = buf.toImage().convertToFormat(QImage::Format_RGB888);
    }
}


void Layout::imageClear()
{
    m_image = new QImage;

    scene = new Scene();
    QPixmap buf = QPixmap::fromImage(*m_image);
    scene->addPixmap(buf);
    grid1->setScene(scene);

}
