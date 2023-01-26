#include "dialog.h"
#include "ui_dialog.h"

#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;
using namespace cv;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    /*첫번째 이미지와 두번째 이미지 파일을 여는 다이얼로그 호출*/
    QString firstImageFile;
    QString secondImageFile;

    /*불러오는 이미지 파일의 경로가 한개라도 없을 경우*/
    while(firstImageFile.isEmpty() || secondImageFile.isEmpty()){
        firstImageFile = QFileDialog::getOpenFileName(this, "open first Image File", "./blending", "File(*.*)");
        secondImageFile = QFileDialog::getOpenFileName(this, "open Second Image File", "./blending", "File(*.*)");
    }

    /*opencv로 두장의 이미지 읽기*/
    image1 = imread(firstImageFile.toStdString(), IMREAD_COLOR);
    image2 = imread(secondImageFile.toStdString(), IMREAD_COLOR);

    /*image1, image2의 이미지 유무 판단*/
    CV_Assert(!(image1.empty() || image2.empty()));

    /*읽어온 이미지의 크기를 변경하는 이미지 생성(resize함수 내에서는 INTER_LAYER가 있음.)*/
    cv::resize(image1, reImage1, Size(960, 640));
    cv::resize(image2, reImage2, Size(960, 640));


    /*슬라이더 이동 값에 따른 블랜딩 효과 처리 connect*/
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onBlending(int)));

    /*슬라이더의 범위*/
    ui->horizontalSlider->setRange(0, 10);

    /*슬라이더의 눈금을 11개의 표시하여 0부터 10까지 표시*/
    ui->horizontalSlider->setTickPosition(QSlider::TicksAbove);

    /*다이얼로그의 크기를 960x640 크기로 조정*/
    this->resize(960, 640);
    //ui->horizontalSlider->setValue(0);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onBlending(int value)
{
    //qDebug() << "slider action : " << value;
    update();

    /*blending 결과 이미지 Mat 생성*/
    Mat result;

    /*alpha 값은 들어오는 value(0 ~ 10)를 10으로 나눈 값*/
    double alpha = (double)value / 10;
    /*이미지의 가중치 alpha값에 맞추어 resize된 image1, image2의 0에 가까울 수록 image1을 선명하게 표시*/
    addWeighted(reImage1, 1 - alpha, reImage2, alpha, 0, result);

    /*변경된 이미지 쓰기, 저장할 이미지도 PNG형식으로 저장하기 위해 ImwriteFlags 조정*/
    imwrite("./blending/result.png", result, vector<int>(ImwriteFlags::IMWRITE_PNG_BILEVEL));

    /*현재 폴더에 위치한 result.bmp를 pix변수에 설정*/
    QPixmap pix("./blending/result.png");
#if 1
    /*form의 레이블 위젯에서 이미지를 출력*/
    ui->label_pic->setPixmap(pix);
#else
    /*라벨의 가로 세로의 길이에 맞추어 resize*/
    //    int w = ui->label_pic->width();
    //    int h = ui->label_pic->height();
    //    ui->label_pic->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
#endif
}

