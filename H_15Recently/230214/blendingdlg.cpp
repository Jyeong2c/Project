#include "blendingdlg.h"
#include "ui_blendingdlg.h"

#define QT_IMPLICIT_QFILEINFO_CONSTRUCTION

#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QResizeEvent>
#include <QListWidgetItem>
#include <QGraphicsScene>


using namespace std;
using namespace cv;


BlendingDlg::BlendingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlendingDlg)
{
    ui->setupUi(this);

    uiSetting();            // 초기 UI 설정
    loadImages();           // listWidget 이미지 출력

    /*다이얼로그 박스에 타이틀 버튼 활성화*/
    setWindowFlag(Qt::WindowMaximizeButtonHint);    //최소화 버튼
    setWindowFlag(Qt::WindowMinimizeButtonHint);    //최대화 버튼
    setWindowFlag(Qt::WindowCloseButtonHint);       //닫기 버튼


    setWindowTitle("Blend Result");         // 다이얼로그 타이틀 설정

    /*sceneImage new 할당*/
    sceneImage = new QGraphicsScene();
    image = new QImage();

    /*슬라이더 이동 값에 따른 블랜딩 효과 처리 connect*/
    connect(ui->alphaSlider, SIGNAL(valueChanged(int)), this, SLOT(onBlending(int)));
}

BlendingDlg::~BlendingDlg()
{
    delete ui;
    delete sceneImage;
    delete image;
}



/*선택된 이미지를 받는 setImage1,2 함수*/
void BlendingDlg::setImage1(const QString& _image1)
{
    image1 = imread(QString("./Images/%1").arg(_image1).toStdString(), IMREAD_COLOR);       // image1 변수에 ./Image/ 내부의 이미지 파일 읽기
}
void BlendingDlg::setImage2(const QString& _image2)
{
    image2 = imread(QString("./Images/%1").arg(_image2).toStdString(), IMREAD_COLOR);       // image2 변수에 ./Image/ 내부의 이미지 파일 읽기
}

/*listWidget의 이미지를 출력하기 위한 로드 함수*/
void BlendingDlg::loadImages()
{
    update();
    QDir saveDir("./SaveImage/");   // 저장된 이미지 경로

    /*환자의 ID별로 이미지리스트를 출력*/
    QDir dir("./Images/");

    copyPath(saveDir.dirName(), dir.dirName()); // SaveImage 디렉토리 내부 파일들을 Image에 복사

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot); // 파일 정보 변수

    /*해당 리스트 위젯을 클리어 한 후 환자의 이미지 리스트를 listWidget에 사진아이콘을 첨부하면서 출력*/
    ui->imageListWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon("./Images/" + fileInfoList.at(i).fileName())
                                                    , NULL, ui->imageListWidget);
        /*해당 아이콘 이미지를 클릭 시 트래킹으로 활성화하여 클릭시 해당 grid에 출력*/
        item->setStatusTip("./Images/" + fileInfoList.at(i).fileName());
        ui->imageListWidget->addItem(item);
    };
    update();
}

/* blending dialog 내부 ui 설정, 비율 설정 함수 */
void BlendingDlg::uiSetting()
{
    /*imageListWidget 의 설정을 변경하기 위한 set함수 목록*/
    ui->imageListWidget->setIconSize(QSize(120, 80));
    ui->imageListWidget->setResizeMode(QListWidget::Adjust);
    ui->imageListWidget->setFlow(QListWidget::LeftToRight);
    ui->imageListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /*blending 결과 이미지와 listWidget을 구분한 부분을 splitter로 분리*/
    QList<int> list;
    list << 400 << 150;
    ui->splitter->setSizes(list);
}

/*다이얼로그의 사이즈가 조정될 때마다 실시간으로 다이얼로그의 너비와 높이가 측정됨*/
void BlendingDlg::resizeEvent(QResizeEvent *e)
{
    /*Blending 되는 아이템의 크기는 그래픽스의 viewport()->size의 크기로 바운드 랙팅 됨*/
    QSize itemSize = ui->pixelDataGraphics->viewport()->size();
    /*pixmap정보는 Images directory의 result.png 데이터 로드(영상처리중 이미지를 그대로 로드)*/
    QPixmap pix("./Images/result.png");

    /*해당 Scene의 아이템을 가져와서 pixItem변수에 QGraphicsPixmapItem을 동적 할당*/
    foreach(auto item, sceneImage->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(pix.scaled(itemSize, Qt::IgnoreAspectRatio));    // pixItem의 setPixmap함수를 사용하여 result.png를 itemSize만큼 scaling, 옵션은 Qt::IgnoreAspecRatio
        sceneImage->setSceneRect(pixItem->sceneBoundingRect());         // sceneImage의 바운딩 직선을 반환함
    }

    QDialog::resizeEvent(e);
}

/*슬라이더 위젯을 움직일 때 마다 가중치 값을 받는 함수*/
void BlendingDlg::onBlending(int value)
{
    update();

    /*image1과 image2가 비어 있는 경우 return 반환*/
    if(image1.empty() || image2.empty()){
        return;
    }

    CV_Assert(!(image1.empty() || image2.empty()));         // image1, image2의 이미지 유무 판단
    ui->alphaSlider->setRange(0, 10);                       // 슬라이더의 범위
    ui->alphaSlider->setTickPosition(QSlider::TicksAbove);  // 슬라이더의 눈금을 11개의 표시하여 0부터 10까지 표시

    /*blending 결과 이미지 Mat 생성*/
    Mat result;         // 결과 이미지 변수 선언

    /*읽어온 이미지의 크기를 변경하는 이미지 생성(resize함수 내에서는 INTER_LAYER가 있음.)*/
    cv::resize(image1, reImage1, Size(ui->pixelDataGraphics->width() - 5, ui->pixelDataGraphics->height() - 5));
    cv::resize(image2, reImage2, Size(ui->pixelDataGraphics->width() - 5, ui->pixelDataGraphics->height() - 5));

    double alpha = (double)value / 10;       // alpha 값은 들어오는 value(0 ~ 10)를 10으로 나눈 값
    /*이미지의 가중치 alpha값에 맞추어 resize된 image1, image2의 0에 가까울 수록 image1을 선명하게 표시*/
    addWeighted(reImage1, 1 - alpha, reImage2, alpha, 0, result);

    /*변경된 이미지 쓰기, 저장할 이미지도 PNG형식으로 저장하기 위해 ImwriteFlags 조정*/
    imwrite("./Images/result.png", result, vector<int>(ImwriteFlags::IMWRITE_PNG_BILEVEL));
    image->load("./Images/result.png");      // 해당 경로에 결과 이미지 호출

    /*이미지에 어떤 정보도 들어가지 않으면 return*/
    if(image->isNull()) {
        return;
    }

    /*그래픽스 위의 Scene을 세팅*/
    ui->pixelDataGraphics->setScene(sceneImage);
    ui->pixelDataGraphics->fitInView(sceneImage->sceneRect(), Qt::IgnoreAspectRatio);

    /*QGraphicsPixmapItem 으로 view->scene->item으로 이미지를 생성*/
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    sceneImage->addItem(item);
    ui->pixelDataGraphics->show();
}

/*listWidget을 클릭 시 이미지 호출*/
void BlendingDlg::on_imageListWidget_clicked(const QModelIndex &index)
{
    /*./Images의 디렉토리의 이미지 데이터를 찾음*/
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    /*파일정보 리스트에 png, jpg, bmp파일들만 리스트정보에 넣어둠*/
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    QString fileInfo = fileInfoList.at(index.row()).fileName();   // 파일이름만 받는 변수 선언후 정의

    /*opencv로 두장의 이미지 읽기*/
    if(imageFlag == 0) {
        /*플래그가 0인경우 image1에 listWidget에 선택된 이미지를 부르기*/
        ui->ImageOneLineEdit->setText(fileInfo);
        setImage1(fileInfo);
        imageFlag = 1;
    }
    else if(imageFlag == 1) {
        /*플래그가 1인 경우 image2에 listWidget에 선택된 이미지를 부르기*/
        ui->ImageTwoLineEdit->setText(fileInfo);
        setImage2(fileInfo);
        imageFlag = 0;
    }
}


/*파일과 디렉토리를 복사하는 함수 srcDir : 복사할 디렉토리, copyDir : 복사받을 디렉토리*/
void BlendingDlg::copyPath(QString srcDir, QString copyDir)
{
    QDir dir(srcDir);       // 복사할 디렉토리를 dir변수로 명시

    /*복사할 디렉토리가 없는 경우*/
    if(!dir.exists()){
        qDebug() << "Not exist directory!!";
        return;
    }

    /*복사할 디렉토리 내부의 디렉토리들을 복사받을 디렉토리 내부에 디렉토리들을 저장*/
    foreach(QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
        /*파일 경로를 구분하기 위한 QDir::separator()*/
        QString copy_path = copyDir + QDir::separator() + d;
        dir.mkdir(copy_path);
        /*재귀함수로 해당 디렉토리안의 디렉토리와 파일들을 복사*/
        copyPath(srcDir + QDir::separator() + d, copy_path);
    }

    /*복사할 디렉토리 내부의 파일들을 복사받을 디렉토리 내부에 파일들을 저장*/
    foreach(QString f, dir.entryList(QDir::Files)){
        QFile::copy(srcDir + QDir::separator() + f, copyDir + QDir::separator() + f);
    }
}



