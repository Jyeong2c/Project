#include "blending.h"
#include "ui_blending.h"

#include <QPixmap>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QResizeEvent>

using namespace std;
using namespace cv;

Blending::Blending(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Blending)
{
    ui->setupUi(this);

    /*첫번째 이미지와 두번째 이미지 파일을 여는 다이얼로그 호출*/
    QString firstImageFile;
    QString secondImageFile;

    /*불러오는 이미지 파일의 경로가 한개라도 없을 경우*/
    while(firstImageFile.isEmpty() || secondImageFile.isEmpty()){
        firstImageFile = QFileDialog::getOpenFileName(this, "open first Image File", "./Images", "File(*.*)");
        secondImageFile = QFileDialog::getOpenFileName(this, "open Second Image File", "./Images", "File(*.*)");
    }

    /*opencv로 두장의 이미지 읽기*/
    image1 = imread(firstImageFile.toStdString(), IMREAD_COLOR);
    image2 = imread(secondImageFile.toStdString(), IMREAD_COLOR);

    /*image1, image2의 이미지 유무 판단*/
    CV_Assert(!(image1.empty() || image2.empty()));

    /*슬라이더 이동 값에 따른 블랜딩 효과 처리 connect*/
    connect(ui->alphaSlider, SIGNAL(valueChanged(int)), this, SLOT(onBlending(int)));

    /*슬라이더의 범위*/
    ui->alphaSlider->setRange(0, 10);

    /*슬라이더의 눈금을 11개의 표시하여 0부터 10까지 표시*/
    ui->alphaSlider->setTickPosition(QSlider::TicksAbove);

    /*다이얼로그의 크기를 960x640 크기로 조정*/
    //this->resize(ui->imageLabel->width(), ui->imageLabel->height());
    //ui->horizontalSlider->setValue(0);

    /*다이얼로그 박스에 타이틀 버튼 활성화*/
    //setWindowFlag(Qt::CustomizeWindowHint);
    setWindowFlag(Qt::WindowMaximizeButtonHint);
    setWindowFlag(Qt::WindowMinimizeButtonHint);
    setWindowFlag(Qt::WindowCloseButtonHint);

    /*다이얼로그 타이틀 선정*/
    setWindowTitle("Blend Result");

    /*sceneImage new 할당*/
    sceneImage = new QGraphicsScene();
    image = new QImage();


}

Blending::~Blending()
{
    delete ui;
}

/*다이얼로그의 사이즈가 조정될 때마다 실시간으로 다이얼로그의 너비와 높이가 측정됨*/
void Blending::resizeEvent(QResizeEvent *e)
{
    qDebug() << "Dialog::resizeEvent()" << e->size();
    ui->pixelDataGraphics->resize(e->size());
    sceneImage->setSceneRect(ui->pixelDataGraphics->viewport()->rect());

    QSize itemSize = ui->pixelDataGraphics->viewport()->size();


    QPixmap pix("./Images/result.png");

    foreach(auto item, sceneImage->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        /**/
        pixItem->setPixmap(pix.scaled(itemSize, Qt::IgnoreAspectRatio));
        sceneImage->setSceneRect(pixItem->sceneBoundingRect());
    }

    qDebug() << "View: " << ui->pixelDataGraphics->width() << "," << ui->pixelDataGraphics->height();
    qDebug() << "Viewport: " << ui->pixelDataGraphics->viewport()->width() << "," << ui->pixelDataGraphics->viewport()->height();
    qDebug() << "Scene: " << sceneImage->sceneRect().width() << "," << sceneImage->sceneRect().height();

    //sceneImage->clear();

    //sceneImage->addRect(0, 0, sceneImage->sceneRect().width()-1, sceneImage->sceneRect().height()-1);
    QDialog::resizeEvent(e);
}

void Blending::onBlending(int value)
{
    //qDebug() << "slider action : " << value;
    update();

    /*blending 결과 이미지 Mat 생성*/
    Mat result;

    /*읽어온 이미지의 크기를 변경하는 이미지 생성(resize함수 내에서는 INTER_LAYER가 있음.)*/
    cv::resize(image1, reImage1, Size(ui->pixelDataGraphics->width() - 5, ui->pixelDataGraphics->height() - 5));
    cv::resize(image2, reImage2, Size(ui->pixelDataGraphics->width() - 5, ui->pixelDataGraphics->height() - 5));


    /*alpha 값은 들어오는 value(0 ~ 10)를 10으로 나눈 값*/
    double alpha = (double)value / 10;
    /*이미지의 가중치 alpha값에 맞추어 resize된 image1, image2의 0에 가까울 수록 image1을 선명하게 표시*/
    addWeighted(reImage1, 1 - alpha, reImage2, alpha, 0, result);

    /*변경된 이미지 쓰기, 저장할 이미지도 PNG형식으로 저장하기 위해 ImwriteFlags 조정*/
    imwrite("./Images/result.png", result, vector<int>(ImwriteFlags::IMWRITE_PNG_BILEVEL));

    /*현재 폴더에 위치한 result.bmp를 pix변수에 설정*/
    //QPixmap pix("./Images/result.png");
#if 0
    /*form의 레이블 위젯에서 이미지를 출력*/
    ui->imageLabel->setPixmap(pix);
#else
    /*라벨의 가로 세로의 길이에 맞추어 resize*/
    //    int w = ui->imageLabel->width();
    //    int h = ui->imageLabel->height();
    //    ui->imageLabel->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    image->load("./Images/result.png");

    if(image->isNull())
    {
        qDebug() << "Image is Null!";
        return;
    }

    //sceneImage = new QGraphicsScene() ;
    ui->pixelDataGraphics->setScene(sceneImage);
    ui->pixelDataGraphics->fitInView(sceneImage->sceneRect(), Qt::IgnoreAspectRatio);

    /*QGraphicsPixmapItem 으로 view->scene->item으로 이미지를 생성*/
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    sceneImage->addItem(item);
    ui->pixelDataGraphics->show();
#endif
}
