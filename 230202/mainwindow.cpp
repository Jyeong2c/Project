#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
//#include "fmx.h"
#include "maxlayout.h"
#include "keyfeaturesform.h"
#include "teethform.h"
//#include "menuitem.h"
#include "view.h"
#include "scene.h"

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;



/* 환자 정보 DB */
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>


#include <QSplitter>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QPalette>
#include <QMdiSubWindow>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPoint>
#include <QRectF>
#include <QHash>
#include <QStackedWidget>
#include <QImage>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>

#include <QSettings>



#define sheetWhite "background:rgb(255, 255, 255)"
#define sheetNavy "background:rgb(32, 56, 100)"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)/*, newScene(nullptr)*/
{    
    ui->setupUi(this);

    /* WindowTitle 지정 */
    stackWidget = new QStackedWidget(this);
    stackWidget->setWindowTitle(tr("2 X 2 Grid"));
    connect(stackWidget, SIGNAL(destroyed()), stackWidget, SLOT(deleteLater()));

    keyFeatures = new KeyFeaturesForm(this);                                              // 주요 기능 클래스
    connect(keyFeatures, SIGNAL(destroyed()), keyFeatures, SLOT(deleteLater()));


    customLayout = new Layout(this);                                                // 레이아웃 화면
    connect(customLayout, SIGNAL(destroyed()), customLayout, SLOT(deleteLater()));
    myMaxlayout = new Maxlayout(this);                                              // 최대 창
    connect(myMaxlayout, SIGNAL(destroyed()), myMaxlayout, SLOT(deleteLater()));


    teeth = new TeethForm();
    connect(teeth, SIGNAL(destroyed()), teeth, SLOT(deleteLater()));


    stackWidget->insertWidget(0, customLayout);
    stackWidget->insertWidget(1, myMaxlayout);
    stackWidget->setCurrentIndex(0);
    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(stackWidget);
    stackWidget->showMaximized();
    //    ui->mdiArea->addSubWindow(fmx);
    //cw->setStyleSheet("background:rgb(32, 56, 100)");
    cw->setStyleSheet("background:rgb(0, 0, 0)");
    ui->mdiArea->setActiveSubWindow(cw);


    QVBoxLayout *KeyLayout = new QVBoxLayout();
    KeyLayout->addWidget(keyFeatures);
    ui->frame->setLayout(KeyLayout);


    styleColor();                                   // ui 색상 조절 및 크기 조절 함수
    loadImages();                                   // listWidget에 이미지 로드
    patientLoad();                                  // 환자 정보 DB 함수

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    /* 시그널 슬롯은 위치가 중요 동적할당(new)보다 밑에 있을 것 */
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));
    connect(customLayout, SIGNAL(sig_size(QGraphicsView*)), SLOT(layoutSizeChanged(QGraphicsView*)));
    connect(myMaxlayout, SIGNAL(max_sig_size(QGraphicsView*)), SLOT(DoubleWidget(QGraphicsView*)));
    connect(customLayout, SIGNAL(sig_widgetbyDClick(QGraphicsView*)), SLOT(DoubleWidget(QGraphicsView*)));
    connect(myMaxlayout->viewQuit, SIGNAL(clicked()), SLOT(previousScreen()));
    connect(myMaxlayout, SIGNAL(sig_clicked(QPointF)), customLayout, SLOT(showMenu(QPointF)));
    connect(keyFeatures, SIGNAL(sig_keyCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)),
            customLayout, SLOT(slot_keyCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)));

    connect(teeth, SIGNAL(sig_11(QPixmap*)), customLayout, SLOT(slot_11(QPixmap*)));
    //    connect(teeth, SIGNAL(sig_11(QLabel*)), customLayout, SLOT(slot_11(QLabel*)));


    connect(customLayout->zoomInAction, SIGNAL(triggered()), SLOT(on_zoomInToolButton_clicked()));
    connect(customLayout->zoomOutAction, SIGNAL(triggered()), SLOT(on_zoomOutToolButton_clicked()));
    connect(customLayout->leftRotateAction, SIGNAL(triggered()), SLOT(on_leftRotateToolButton_clicked()));
    connect(customLayout->rightRotateAction, SIGNAL(triggered()), SLOT(on_rightRotateToolButton_clicked()));
    connect(customLayout->sourceSizeAction, SIGNAL(triggered()), SLOT(on_sourceSizePushButton_clicked()));
    connect(customLayout->layoutImageClearAction, SIGNAL(triggered()), SLOT(on_layoutClearPushButton_clicked()));



    /* Processing */
    //    ui->brightnessSlider->setRange(-100,100);
    ui->brightnessSlider->setRange(-8,8);
    ui->brightnessSlider->setSliderPosition(0);
    ui->brightnessSlider->setPageStep(1);
    ui->brightnessSlider->setTickPosition(QSlider::TicksAbove);

    ui->contrastSlider->setRange(0,3);
    ui->contrastSlider->setSliderPosition(0);
    ui->contrastSlider->setPageStep(1);
    ui->contrastSlider->setTickPosition(QSlider::TicksAbove);
    currentImage();
}

MainWindow::~MainWindow()
{
    delete ui;
    QSqlDatabase clDB = QSqlDatabase::database("patientDB");            // 데이터베이스 닫기
    if(clDB.isOpen()){                                                  // 만약 파일이 열리면
        patientQueryModel->submitAll();
        delete patientQueryModel;
        clDB.close();
    }
}


void MainWindow::loadImages()
{
    QDir dir(".");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->listWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, ui->listWidget);
        item->setStatusTip(fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
}

void MainWindow::selectItem(QListWidgetItem *item)
{
    /* view의 위치를 선택하지 않고 listWidget에 있는 이미지를 먼저 선택한 경우 */
    if(customLayout->g == false) {
        if(cnt == 0) {
            customLayout->grid = customLayout->grid1;
            customLayout->scene = customLayout->scene1;
            cnt ++;

            customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            customLayout->grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }
        else if(cnt == 1) {
            customLayout->grid = customLayout->grid2;
            customLayout->scene = customLayout->scene2;
            cnt ++;

            customLayout->grid2->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }
        else if(cnt == 2) {
            customLayout->grid = customLayout->grid3;
            customLayout->scene = customLayout->scene3;
            cnt ++;

            customLayout->grid3->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");

        }
        else if(cnt == 3) {
            customLayout->grid = customLayout->grid4;
            customLayout->scene = customLayout->scene4;
            cnt ++;

            customLayout->grid4->setStyleSheet("border: 0.5px solid rgb(0,255,0)");
            customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid2->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
            customLayout->grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
        }

        foreach(auto item, customLayout->scene->items()) {
            customLayout->scene->removeItem(item);
            delete item;
        }
        customLayout->scene->setBackgroundBrush(Qt::black);
        QSize size = customLayout->grid->viewport()->size();
        QGraphicsPixmapItem *pixItem = customLayout->scene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio));
        customLayout->grid->setAlignment(Qt::AlignCenter);
        customLayout->scene->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->grid->update();
        itemHash[pixItem] = item->statusTip();
        customLayout->grid->resetTransform();
        m_image = QImage(pixItem->pixmap().toImage());
    }

    /* view의 위치를 클릭하고 이미지를 선택한 경우 */
    else if(customLayout->g == true) {
        customLayout->grid->resetTransform();
        foreach(auto item, customLayout->scene->items()) {
            customLayout->scene->removeItem(item);
            delete item;
        }
        customLayout->scene->setBackgroundBrush(Qt::black);
        QSize size = customLayout->grid->viewport()->size();
        QGraphicsPixmapItem *pixItem = customLayout->scene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio));
        customLayout->grid->setAlignment(Qt::AlignCenter);
        customLayout->scene->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->grid->update();
        itemHash[pixItem] = item->statusTip();
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_image = QImage(pixItem->pixmap().toImage());
    }
}


void MainWindow::layoutSizeChanged(QGraphicsView *grid)
{
    //    Q_UNUSED(grid);

    /* view 1,2,3,4의 크기 */
    QSize itemSize1 = customLayout->grid1->viewport()->size();
    QSize itemSize2 = customLayout->grid2->viewport()->size();
    QSize itemSize3 = customLayout->grid3->viewport()->size();
    QSize itemSize4 = customLayout->grid4->viewport()->size();

    foreach(auto item, customLayout->scene1->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize1, Qt::KeepAspectRatio));
        //        QPixmap image = QPixmap(QPixmap::fromImage(m_image));
        //        image.scaled(itemSize1, Qt::KeepAspectRatio);
        customLayout->scene1->setSceneRect(pixItem->sceneBoundingRect());
    }
    foreach(auto item, customLayout->scene2->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize2, Qt::KeepAspectRatio));
        customLayout->scene2->setSceneRect(pixItem->sceneBoundingRect());
    }

    foreach(auto item, customLayout->scene3->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize3, Qt::KeepAspectRatio));
        customLayout->scene3->setSceneRect(pixItem->sceneBoundingRect());
    }

    foreach(auto item, customLayout->scene4->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize4, Qt::KeepAspectRatio));
        customLayout->scene4->setSceneRect(pixItem->sceneBoundingRect());
    }

    customLayout->grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void MainWindow::DoubleWidget(QGraphicsView* grid)
{
    ui->line->hide();

    QList<int> sizes2;
    sizes2 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter2->setSizes(sizes2);

    myMaxlayout->setNewSc(customLayout->grid->scene());

    stackWidget->setCurrentIndex(1);

    /* 더블 클릭했을 때 view의 크기 */
    QSize itemSize5 = myMaxlayout->maxNewGrid->viewport()->size();

    foreach(auto item, myMaxlayout->maxNewSc->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize5, Qt::KeepAspectRatio));
        myMaxlayout->maxNewSc->setSceneRect(pixItem->sceneBoundingRect());
    }
    myMaxlayout->viewQuit->setGeometry(myMaxlayout->maxNewGrid->width()-45, 10, 30, 30);
    myMaxlayout->maxNewGrid->resetTransform();
}

void MainWindow::previousScreen()
{
    stackWidget->setCurrentIndex(0);

    ui->line->show();

    QList<int> sizes2;
    sizes2 << 700 << 100;
    ui->splitter2->setSizes(sizes2);

    customLayout->grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QSize itemSize1 = customLayout->grid1->viewport()->size();
    QSize itemSize2 = customLayout->grid2->viewport()->size();
    QSize itemSize3 = customLayout->grid3->viewport()->size();
    QSize itemSize4 = customLayout->grid4->viewport()->size();


    foreach(auto item, customLayout->scene1->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize1, Qt::KeepAspectRatio));
        customLayout->scene1->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->grid1->resetTransform();
    }

    foreach(auto item, customLayout->scene2->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize2, Qt::KeepAspectRatio));
        customLayout->scene2->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->grid2->resetTransform();
    }

    foreach(auto item, customLayout->scene3->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize3, Qt::KeepAspectRatio));
        customLayout->scene3->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->grid3->resetTransform();
    }

    foreach(auto item, customLayout->scene4->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize4, Qt::KeepAspectRatio));
        customLayout->scene4->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->grid4->resetTransform();
    }
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    //    qDebug("widget changed");
    //    qDebug("위젯 변화에 따른 viewport : %d", customLayout->grid1->viewport()->size());

}

void MainWindow::changeEvent(QEvent *event)
{
    Q_UNUSED(event);
    //    QMainWindow::changeEvent(event);
    //    if(event->type() == QEvent::WindowStateChange) {                 // 윈도우 창 변경 (최소화, 최대화)
    ////        customLayout->grid1->viewport()->size();
    ////        qDebug("window Changed");
    ////        qDebug("윈도우 사이즈 변화에 따른 viewport : %d", customLayout->grid1->viewport()->size());
    //    }
}

void MainWindow::on_layoutClearPushButton_clicked()                    // scene 이미지 초기화
{
    customLayout->grid1->resetTransform();
    customLayout->grid2->resetTransform();
    customLayout->grid3->resetTransform();
    customLayout->grid4->resetTransform();

    customLayout->grid1->scene()->clear();
    customLayout->grid2->scene()->clear();
    customLayout->grid3->scene()->clear();
    customLayout->grid4->scene()->clear();

    cnt = 0;

    customLayout->g = false;
}


/* 환자 정보 DB */
void MainWindow::patientLoad()
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE", "patientConnection");
    DB.setDatabaseName("patientDB.db");                             //데이터베이스 이름설정

    if(DB.open()){                                                 //조건문
        patientQuery = new QSqlQuery(DB);
        //query 문을 이용하여 테이블 생성 및 PK 키 설정
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

/* ui 색상 및 크기 조절 */
void MainWindow::styleColor()
{
    ui->mdiArea->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter2->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter3->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter4->setStyleSheet("background:rgb(32, 56, 100)");
    ui->listWidget->setStyleSheet("background:rgb(255, 255, 255)");
    ui->patientTableView->setStyleSheet("background:rgb(255, 255, 255)");
    ui->doctorNameLineEdit->setStyleSheet("background:rgb(180, 199, 231)");
    //ui->line->setStyleSheet("background:rgb(255, 255, 255)");
    ui->line->setStyleSheet(sheetWhite);


    /* Painter */
    ui->tabWidget->setStyleSheet(sheetNavy);
    ui->brushToolButton->setStyleSheet(sheetWhite);
    ui->spinBox->setStyleSheet(sheetWhite);;
    ui->circleToolButton->setStyleSheet(sheetWhite);
    ui->triangleToolButton->setStyleSheet(sheetWhite);
    ui->rectangleToolButton->setStyleSheet(sheetWhite);
    ui->arrowToolButton->setStyleSheet(sheetWhite);
    ui->colorToolButton->setStyleSheet(sheetWhite);

    /* Processing */
    //ui->brightToolButton->setStyleSheet(sheetWhite);
    //ui->brightnessToolButton->setStyleSheet(sheetWhite);
    ui->sharpenToolButton->setStyleSheet(sheetWhite);
    ui->inversionToolButton->setStyleSheet(sheetWhite);
    ui->horizontalFlipToolButton->setStyleSheet(sheetWhite);
    ui->verticalFlipToolButton->setStyleSheet(sheetWhite);
    ui->zoomInToolButton->setStyleSheet(sheetWhite);
    ui->zoomOutToolButton->setStyleSheet(sheetWhite);
    ui->panningToolButton->setStyleSheet(sheetWhite);
    ui->leftRotateToolButton->setStyleSheet(sheetWhite);
    ui->rightRotateToolButton->setStyleSheet(sheetWhite);

    /* Measure */
    ui->rulerToolButton->setStyleSheet(sheetWhite);
    ui->measureToolButton->setStyleSheet(sheetWhite);
    ui->protractorToolButton->setStyleSheet(sheetWhite);
    ui->implantToolButton->setStyleSheet(sheetWhite);
    //    ui->implantToolButton_2->setStyleSheet(sheetWhite);


    // ui->toolBar->setToolButtonStyle(Qt::ToolBarArea));

    QList<int> sizes;
    sizes << 10 << 1000;
    ui->splitter->setSizes(sizes);

    QList<int> sizes2;
    sizes2 << 700 << 100;
    ui->splitter2->setSizes(sizes2);

    QList<int> sizes3;
    sizes3 << 10 << 2000;
    ui->splitter3->setSizes(sizes3);

    QList<int> sizes4;
    sizes4 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter4->setSizes(sizes4);


    ui->verticalSpacer->changeSize(20, 15);
    ui->verticalSpacer_2->changeSize(20, 40);
    ui->verticalSpacer_3->changeSize(20, 20);
    ui->verticalSpacer_4->changeSize(20, 20);
    ui->horizontalSpacer_8->changeSize(20, 12);
    ui->horizontalSpacer_9->changeSize(10,10);
    ui->horizontalSpacer_10->changeSize(10,10);
    ui->sourceSizePushButton->setFixedSize(70, 40);


    /* toolButton 크기 조절 */
    ui->brushToolButton->setIconSize(QSize(50,40));
    ui->brushToolButton->setText("Brush");
    ui->brushToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->colorToolButton->setIconSize(QSize(100,30));
    ui->colorToolButton->setText("Color");
    ui->colorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}




void MainWindow::on_zoomInToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->scale(1.25, 1.25);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    myMaxlayout->maxNewGrid->scale(1.25, 1.25);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_zoomOutToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->scale(0.8, 0.8);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    myMaxlayout->maxNewGrid->scale(0.8, 0.8);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_leftRotateToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->rotate(-90);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    myMaxlayout->maxNewGrid->rotate(-90);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_rightRotateToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->rotate(90);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    myMaxlayout->maxNewGrid->rotate(90);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_sourceSizePushButton_clicked()
{
    if(customLayout->g == true)
        customLayout->grid->resetTransform();

    myMaxlayout->maxNewGrid->resetTransform();
}

void MainWindow::slot_doctorInfo(QString DoctorID)
{
    ui->doctorNameLineEdit->setText(DoctorID);

    keyFeatures->slot_doctorInfo(DoctorID);
    keyFeatures->insertInfo(DoctorID);
    //    keyFeatures->updateInfo(DoctorID);
}


void MainWindow::on_implantToolButton_clicked()
{
    teeth->show();

    //    if(customLayout->g == true) {
    //        customLayout->scene->setCurrentShape(Scene::Image);
    //    }
    //    myMaxlayout->maxNewSc->clearItems();

    //    if(customLayout->g == true) {
    //        customLayout->scene->setCurrentShape(Scene::Implant);
    //    }
}





void MainWindow::on_brightnessSlider_valueChanged(int intensity)
{
    brightness(intensity);
}


void MainWindow::on_contrastSlider_valueChanged(int alpha)
{
    contrast(alpha);
}


void MainWindow::on_sharpenToolButton_clicked()
{
    sharpen();
}


void MainWindow::on_inversionToolButton_clicked()
{
    inversion();
}


void MainWindow::on_verticalFlipToolButton_clicked()
{
    verticalFlip();
}


void MainWindow::on_horizontalFlipToolButton_clicked()
{
    horizontalFlip();
}

void MainWindow::sharpen()
{
    inten +=0.5;

    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            return;
        }
        if(cnt == 5){
            cnt = 0;
            inten = 0;
            qDebug()<<inten;
            auto item = customLayout->scene->items().first();
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            pixItem->setPixmap(QPixmap::fromImage(m_image));
        }
        //                if(inten == intenMax){
        //                    inten =0;
        //                }

        cnt++;
        qDebug() << cnt;

        qDebug()<<"1";
        auto item = customLayout->scene->items().first();
        //        auto copy_item = item->topLevelItem();

        pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

        pix_image = QPixmap(pixItem->pixmap());
        QImage dis_image = QImage(pix_image.toImage());
        //        QImage dis_image = QImage(m_image);

        dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
        cv::Mat mat = cv::Mat(dis_image.height(),
                              dis_image.width(),
                              CV_8UC3,
                              dis_image.bits(),
                              dis_image.bytesPerLine());
        //1. 입력이미지 2.출력이미지  3.커널의 크기  4.가우스 커널 표준편차를 나타내는 double형 변수
        Mat smoothed;
        GaussianBlur(mat, smoothed,Size(5,5),0);
        Mat sharpenImage = mat + (mat- smoothed) * inten;

        outImg = QImage(sharpenImage.data,
                        sharpenImage.cols,
                        sharpenImage.rows,
                        sharpenImage.step,
                        QImage::Format_RGB888).copy();

        outPixImage = QPixmap::fromImage(outImg);
        //        customLayout->scene->addPixmap(buf);
        pixItem->setPixmap(outPixImage);

    }

}
void MainWindow::brightness(int intensity)
{
    test++;
    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            return;
        }
        //        foreach(auto i, list.last()){
        //          QGraphicsPixmapItem * pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
        //            m_imageCopy = QImage(image);
        //            QPixmap test = QPixmap(*m_pixmapGroup));
        //            }
        //        }
        foreach (auto i, customLayout->scene->items()){
            QGraphicsPixmapItem * pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
            pix_image = QPixmap(pixItem->pixmap());

            QImage dis_image = QImage(m_image.copy());
            //        QImage dis_image = QImage(pix_image.toImage());
            dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
            cv::Mat mat = cv::Mat(dis_image.height(),
                                  dis_image.width(),
                                  CV_8UC3,
                                  dis_image.bits(),
                                  dis_image.bytesPerLine());


            Mat brightness;
            mat.convertTo(brightness, -1, 1, intensity);
            //     Mat brightnessLow = mat - 100;

            outImg =QImage(brightness.data,
                           brightness.cols,
                           brightness.rows,
                           brightness.step,
                           QImage::Format_RGB888).copy();


            outPixImage = QPixmap::fromImage(outImg);
            //                customLayout->scene->addPixmap(buf);
            pixItem->setPixmap(outPixImage);
        }
        //        if(test == 1){
        //        auto item = customLayout->scene->items().first();

        //        pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

        //        pix_image = QPixmap(pixItem->pixmap());
        //        //        QPixmap pixmap = pix_image;
        //        //            m_image = QImage(pix_image.toImage());
        //        //           QImage m_imageCopy = QImage(m_image.copy());

        //        QImage dis_image = QImage(pix_image.toImage());
        //        //        QImage dis_image = QImage(pix_image.toImage());
        //        dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
        //        cv::Mat mat = cv::Mat(dis_image.height(),
        //                              dis_image.width(),
        //                              CV_8UC3,
        //                              dis_image.bits(),
        //                              dis_image.bytesPerLine());


        //        Mat brightness;
        //        mat.convertTo(brightness, -1, 1, intensity);
        //        //     Mat brightnessLow = mat - 100;

        //        outImg =QImage(brightness.data,
        //                       brightness.cols,
        //                       brightness.rows,
        //                       brightness.step,
        //                       QImage::Format_RGB888).copy();


        //        outPixImage = QPixmap::fromImage(outImg);
        //        //                customLayout->scene->addPixmap(buf);
        //        pixItem->setPixmap(outPixImage);
        // }else{
        //            auto item = customLayout->scene->items().first();
        //            pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        //            pix_image = QPixmap(pixItem->pixmap());

        ////            m_image = QImage(pix_image.toImage());
        //            QImage d_image = QImage(pix_image.toImage());

        //            d_image= d_image.convertToFormat(QImage::Format_RGB888);
        //            cv::Mat mat = cv::Mat(d_image.height(),
        //                                  d_image.width(),
        //                                  CV_8UC3,
        //                                  d_image.bits(),
        //                                  d_image.bytesPerLine());


        //            Mat brightness;
        //            mat.convertTo(brightness, -1, 1, intensity);
        //            //     Mat brightnessLow = mat - 100;

        //            outImg =QImage(brightness.data,
        //                           brightness.cols,
        //                           brightness.rows,
        //                           brightness.step,
        //                           QImage::Format_RGB888).copy();


        //            pix_image = QPixmap::fromImage(outImg);
        //            //                customLayout->scene->addPixmap(buf);
        //            pixItem->setPixmap(pix_image);
        //        }
        //       QList<QGraphicsPixmapItem> list;
        //       list.append(pixItem);
        //        m_pixmapGroup = new QGraphicsItemGroup();
        //       m_pixmapGroup->setGroup(pixItem->group());
    }
}
void MainWindow::contrast(int alpha)
{
    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            return;
        }
//        if(ui->contrastSlider->value() == 3){
//            foreach (auto i, customLayout->scene->items()){
//                QGraphicsPixmapItem * pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
//                ui->contrastSlider->setValue(0);
//                pixItem->setPixmap(QPixmap::fromImage(m_image));
//            }
//        }

        foreach (auto i, customLayout->scene->items()){
            QGraphicsPixmapItem * pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
            //        auto item = customLayout->scene->items().constFirst();

            //        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            QPixmap pixmap = QPixmap(pixItem->pixmap());
            QPixmap pixmapcopy = QPixmap(pixmap);

                    m_image= QImage(pixmap.toImage());
                    QImage dis_image = QImage(m_image.copy());
//            QImage dis_image = QImage(pixmap.toImage());

            dis_image = dis_image.convertToFormat(QImage::Format_RGB888);
            cv::Mat mat = cv::Mat(dis_image.height(),
                                  dis_image.width(),
                                  CV_8UC3,
                                  dis_image.bits(),
                                  dis_image.bytesPerLine());

            Mat contrast = mat+(mat -128) * alpha;

            QImage outImg = QImage(contrast.data,
                                   contrast.cols,
                                   contrast.rows,
                                   contrast.step,
                                   QImage::Format_RGB888).copy();

            outPixImage = QPixmap::fromImage(outImg);
            pixItem->setPixmap(outPixImage);
            //        cv::createTrackbar("contrast", "win_name", 0, 100, outImg);
            //        pix_image = QPixmap::fromImage(outImg);
            //        customLayout->scene->addPixmap(buf);
            //        pixItem->setPixmap(pix_image);
        }
    }
}
void MainWindow::inversion()
{
    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            return;
        }

        //        foreach(auto item, customLayout->scene->items()){
        auto item = customLayout->scene->items().first();

        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        //        if(pixItem != nullptr) {
        QPixmap pixmap = pixItem->pixmap();
        //            m_image = QImage(pix_image.toImage());

        QImage dis_image = QImage(pixmap.toImage());

        //m_image = QImage(dis_image);
        dis_image.invertPixels();

        outPixImage = QPixmap::fromImage(dis_image);

        //            customLayout->scene->addPixmap(buf);
        pixItem->setPixmap(outPixImage);
        //            buf.save("123.jpg");
        //            pixItem->pixmap().save("456.jpg");
        //}
    }
}
void MainWindow::verticalFlip()
{
    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            return;
        }
        auto item = customLayout->scene->items().first();

        pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

        QPixmap pixmap = pixItem->pixmap();

        QImage dis_image = QImage(pixmap.toImage());
        //m_image = QImage(pixmap->toImage());
        dis_image.mirror(true,false);
        outPixImage = QPixmap::fromImage(dis_image);
        //        customLayout->scene->addPixmap(buf);
        pixItem->setPixmap(outPixImage);

    }
}
void MainWindow::horizontalFlip()
{
    if(customLayout->g == true){

        if(customLayout->scene->items().empty()){
            return;
        }
        auto item = customLayout->scene->items().first();

        pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

        //        QPixmap * pixmap = new QPixmap(pixItem->pixmap());
        QPixmap pixmap = pixItem->pixmap();

        QImage dis_image = QImage(pixmap.toImage());

        // m_image = QImage(pixmap->toImage());
        //QImage image_to_display = QImage(m_image);
        dis_image.mirror(false,true);

        outPixImage = QPixmap::fromImage(dis_image);
        //            scene->addPixmap(buf.scaled(image_to_display.width(),image_to_display.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));;
        //        customLayout->scene->addPixmap(buf);
        pixItem->setPixmap(outPixImage);
    }
}

void MainWindow::currentImage()
{

    //    if(customLayout->g == true){

    //        if(customLayout->scene->items().empty()){
    //            return;
    //        }

    //        auto item = customLayout->scene->items().first();
    //        pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

    //        QPixmap * pixmap = new QPixmap(pixItem->pixmap());

    //        m_image = QImage(pixmap->toImage());


    //    }
}



void MainWindow::on_processingClear_clicked()
{
    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            return;
        }

        imageClear = QPixmap::fromImage(m_image);
        pixItem->setPixmap(imageClear);
        //        layoutSizeChanged(grid);
        //        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(outPixImage, Qt::KeepAspectRatio));
    }
}


void MainWindow::on_brightnessSlider_sliderReleased()
{
    //    value = ui->brightnessSlider->setValue();
    //    pix_image = QPixmap::fromImage(outImg);
}


void MainWindow::on_actionSave_triggered()
{
    if(customLayout->g == true){
        if(customLayout->scene->items().empty()){
            QMessageBox::information(this,tr("warning"),tr("No search image"));
            return;
        }
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                        QString(),
                                                        tr("Images(*.png)"));
        foreach(auto i, customLayout->scene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
            pixItem->pixmap().save(fileName);
            qDebug("save");
        }
    }
}
