#include "mainwindow.h"
//#include "qgraphicsview.h"
#include "ui_mainwindow.h"
#include "layout.h"
#include "fmx.h"
#include "scene.h"

/* 환자 정보 DB */
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>
#include <QGraphicsView>
#include <QColorDialog>


#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 700 << 70;
    ui->splitter->setSizes(sizes);

    QList<int> sizes2;
    sizes2 << 500 << 130;
    ui->splitter2->setSizes(sizes2);

    QList<int> sizes3;
    sizes3 << 100 << 550;
    ui->splitter3->setSizes(sizes3);

    ui->verticalSpacer->changeSize(20, 25);

    ui->brushToolButton->setIconSize(QSize(50,40));
    ui->brushToolButton->setText("Brush");
    ui->brushToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->colorToolButton->setIconSize(QSize(100,30));
    ui->colorToolButton->setText("Color");
    ui->colorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    /* 환자 정보 DB */
    patientLoad();

    ui->horizontalSlider->setRange(-100,100);
    ui->horizontalSlider->setSliderPosition(0);
    ui->horizontalSlider->setTickPosition(QSlider::TicksAbove);

//    ui->horizontalSlider->





    /* WindowTitle 지정 */
    gridLayout = new Layout(this);
    gridLayout->setWindowTitle(tr("2 X 2 Grid"));
    connect(gridLayout, SIGNAL(destroyed()), gridLayout, SLOT(deleteLater()));

    fmx = new FMX(this);
    fmx->setWindowTitle(tr("14 FMX"));
    connect(fmx, SIGNAL(destroyed()), fmx, SLOT(deleteLater()));

    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(gridLayout);
    ui->mdiArea->addSubWindow(fmx);
    ui->mdiArea->setActiveSubWindow(cw);

    //==========툴 버튼 액션 메뉴 추가=======================================================
    //    QMenu * sharpenMenu = new QMenu(this);

    //    QAction * phaseOne = new QAction("phaseOne",this);
    //    QAction * phaseTwo = new QAction("phaseTwo",this);
    //    QAction * phaseThree = new QAction("phaseThree",this);

    //    sharpenMenu->addAction(phaseOne);
    //    sharpenMenu->addAction(phaseTwo);
    //    sharpenMenu->addAction(phaseThree);
    //    ui->sharpenToolButton->setMenu(sharpenMenu);
    //    ui->sharpenToolButton->setPopupMode(QToolButton::InstantPopup);


    //    connect(phaseOne,SIGNAL(triggered()),gridLayout,SLOT(dark()));
    //    connect(phaseTwo,SIGNAL(triggered()),gridLayout,SLOT(dark1()));
    //    connect(phaseThree,SIGNAL(triggered()),gridLayout,SLOT(dark2()));
    //===============================================================================
    //    connect(this,SIGNAL(requestDark()),gridLayout,SLOT(dark()));
    //    connect(this,SIGNAL(requestDark1()),gridLayout,SLOT(dark1()));
    //    connect(this,SIGNAL(requestDark2()),gridLayout,SLOT(dark2()));

//    connect(this,SIGNAL(requestBrightUp()),gridLayout,SLOT(test()));
//    connect(this,SIGNAL(requestBrightLow()),gridLayout,SLOT(test1()));
//    connect(this,SIGNAL(requestInverstion()),gridLayout,SLOT(inverstion()));
//    connect(this,SIGNAL(requestVerticalFlip()),gridLayout,SLOT(verticalFlip()));
//    connect(this,SIGNAL(requestHorizontalFlip()),gridLayout,SLOT(horizontalFlip()));
//    //    connect(this,SIGNAL(requestBrightUp()),gridLayout,SLOT(brightUp()));
//    //    connect(this,SIGNAL(requestDark()),gridLayout,SLOT(dark()));
//    connect(this,SIGNAL(requestZoomIn()),gridLayout,SLOT(zoomIn()));
//    connect(this,SIGNAL(requestZoomOut()),gridLayout,SLOT(zoomOut()));
//    connect(this,SIGNAL(requestLeftRotate()),gridLayout,SLOT(leftRotate()));
//    connect(this,SIGNAL(requestRightRotate()),gridLayout,SLOT(rightRotate()));
//    //    connect(this,SIGNAL(requestBrightLow()),gridLayout,SLOT())
//    connect(this,SIGNAL(requestImageClear()),gridLayout,SLOT(imageClear()));
//    connect(this,SIGNAL(requestBack()),gridLayout,SLOT(back()));

    connect(ui->sharpenToolButton,SIGNAL(clicked()),gridLayout,SLOT(sharpen()));
    connect(ui->brightToolButton,SIGNAL(clicked()),gridLayout,SLOT(brightnessDownOpenCV()));
    connect(ui->brightnessToolButton,SIGNAL(clicked()),gridLayout,SLOT(brightnessUpOpenCV()));
    connect(ui->inversionToolButton,SIGNAL(clicked()),gridLayout,SLOT(inverstion()));
    connect(ui->verticalFlipToolButton,SIGNAL(clicked()),gridLayout,SLOT(verticalFlip()));
    connect(ui->horizontalFlipToolButton,SIGNAL(clicked()),gridLayout,SLOT(horizontalFlip()));
    connect(ui->zoomInToolButton,SIGNAL(clicked()),gridLayout,SLOT(zoomIn()));
    connect(ui->zoomOutToolButton,SIGNAL(clicked()),gridLayout,SLOT(zoomOut()));
    connect(ui->rightRotateToolButton,SIGNAL(clicked()),gridLayout,SLOT(rightRotate()));
    connect(ui->leftRotateToolButton,SIGNAL(clicked()),gridLayout,SLOT(leftRotate()));
    connect(ui->drawPushButton,SIGNAL(clicked()),gridLayout,SLOT(his()));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),gridLayout,SLOT(brightSlider(int)));
    connect(ui->imagePushButton,SIGNAL(clicked()),gridLayout,SLOT(imageClear()));




}

MainWindow::~MainWindow()
{
    delete ui;
    QSqlDatabase clDB = QSqlDatabase::database("patientDB");  //데이터베이스 닫기
    if(clDB.isOpen()){                                               //만약 파일이 열리면
        patientQueryModel->submitAll();
        delete patientQueryModel;
        clDB.close();
    }
}

void MainWindow::patientLoad()
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("patientDB.db");                             //데이터베이스 이름설정

    if(DB.open()){                                                 //조건문
        patientQuery = new QSqlQuery(DB);
        //query 문을 이용하여 테이블 생성 및 PK 키 설정
        //        patientQuery->exec("DELETE ")

        patientQuery->exec("CREATE TABLE IF NOT EXISTS patient(chartNum INTEGER Primary Key,"
                           "name VARCHAR(20) NOT NULL, age INTEGER,imagePath VARCHAR(20));");

        patientQueryModel = new QSqlTableModel(this, DB);
        patientQueryModel->setTable("patient");
        patientQueryModel->select();

        /*테이블 헤더 설정*/
        patientQueryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ChartNum"));
        patientQueryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        patientQueryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
        patientQueryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("ImagePath"));

        ui->patientTableView->setModel(patientQueryModel);

        ui->patientTableView->hideColumn(3);

        patientQuery->exec("INSERT INTO patient VALUES (1000,'JaeYeong','28','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1001,'Yuna','26','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1002,'Jaehyun','27','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1003,'eunji','29','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1004,'chelly','28','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1005,'brian','26','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1006,'dessery','27','..')");
        patientQuery->exec("INSERT INTO patient VALUES (1007,'eclipse','29','..')");
        patientQueryModel->select();
        ui->patientTableView->resizeColumnsToContents();


    }
}




//void Layout::brush()
//{
//     gridLayout()->grid1-scene->setCurrentShape(Scene::Path);
//     gridLayout->grid1->setScene(scene);

//}



void MainWindow::on_brushToolButton_clicked()
{
    gridLayout->scene->setCurrentShape(Scene::Path);
}


void MainWindow::on_rectangleToolButton_clicked()
{
    gridLayout->scene->setCurrentShape(Scene::Rect);
}


void MainWindow::on_colorToolButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()){
        gridLayout->scene->setCurrentColor(color);
    }
}


void MainWindow::on_doubleSpinBox_valueChanged(double size)
{
    gridLayout->scene->setPaintSize(size);
}


void MainWindow::on_circleToolButton_clicked()
{
    gridLayout->scene->setCurrentShape(Scene::Ellipse);
}


void MainWindow::on_arrowToolButton_clicked()
{
    gridLayout->scene->setCurrentShape(Scene::Line);
}

