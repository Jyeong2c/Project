#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
#include "maxlayout.h"

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

/*서버를 연동하기 위한 Qt 헤더*/
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFileDialog>

/*해당되는 URL로부터 이미지로드를 위한 헤더*/
#include <iostream>
#include <windows.h>
#include <QProgressDialog>
#include <QWebSocketServer>
#include <QMessageBox>
#include <QWebSocket>
#include <QDataStream>
#include <QNetworkInformation>
#include <QSettings>
#include <QCollator>

/*Blending Dialog를 호출하는 헤더*/
#include "blending.h"

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

    customLayout = new Layout(this);

    myMaxlayout = new Maxlayout(this);              // 최대창

    stackWidget->insertWidget(0, customLayout);
    stackWidget->insertWidget(1, myMaxlayout);
    stackWidget->setCurrentIndex(0);
    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(stackWidget);
    stackWidget->showMaximized();
    cw->setStyleSheet("background:rgb(0, 0, 0)");
    ui->mdiArea->setActiveSubWindow(cw);

    styleColor();                                   // ui 색상 조절 및 크기 조절 함수
    //loadImages();                                 // 환자를 선택할 때마다 이미지 리스트를 띄움
    patientLoad();                                  // 환자 정보 DB 함수

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /*Setting 정보 읽어오기*/
    readSettings();

    /* 시그널 슬롯은 위치가 중요 동적할당(new)보다 밑에 있을 것 */
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));
    connect(customLayout, SIGNAL(sig_size(QGraphicsView*)), SLOT(layoutSizeChanged(QGraphicsView*)));
    connect(myMaxlayout, SIGNAL(max_sig_size(QGraphicsView*)), SLOT(DoubleWidget(QGraphicsView*)));
    connect(customLayout, SIGNAL(sig_widgetbyDClick(QGraphicsView*)), SLOT(DoubleWidget(QGraphicsView*)));
    connect(myMaxlayout->viewQuit, SIGNAL(clicked()), SLOT(previousScreen()));
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

    QDir dir("./Images/");
    /*모든 내용(파일)을 포함하여 해당 디렉토리(./Images/)를 제거*/
    dir.removeRecursively();

    /*Setting 정보 저장*/
    writeSettings();
}

void MainWindow::writeSettings()
{
    QSettings settings("./Project.ini" , QSettings::IniFormat);

    settings.beginGroup("Project");
    settings.setValue("OS/os_name", GEO_NAME);
    settings.setValue("NET_WORK/local_ip", hostName + ":" + portNum);
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("INIT/init_settings","-1");
    settings.setValue("ADMIN_CONF/site_cd","-1");
    settings.setValue("ADMIN_CONF/str_cd","-1");
    settings.setValue("ADMIN_CONF/process_type","-1");
    settings.setValue("EQU_CONF/eq_type","-1");
    settings.setValue("EQU_CONF/eq_nm","-1");
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("./Project.ini" , QSettings::IniFormat);

    settings.beginGroup("Project");

}

void MainWindow::loadImages()
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    update();
    /*환자의 ID별로 이미지리스트를 출력*/
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    /*해당 리스트 위젯을 클리어 한 후 환자의 이미지 리스트를 listWidget에 사진아이콘을 첨부하면서 출력*/
    ui->listWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon("./Images/" + fileInfoList.at(i).fileName())
                                                    , NULL, ui->listWidget);
        /*해당 아이콘 이미지를 클릭 시 트래킹으로 활성화하여 클릭시 해당 grid에 출력*/
        item->setStatusTip("./Images/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
    update();
}

void MainWindow::selectItem(QListWidgetItem *item)
{
    qDebug() << "Test";

    /* view의 위치를 선택하지 않고 listWidget에 있는 이미지를 먼저 선택한 경우 */
    if(customLayout->g == false) {
        if(cnt == 0) {
            customLayout->grid = customLayout->grid1;
            customLayout->scene = customLayout->scene1;
            cnt ++;
            qDebug() << "click : 1";
        }
        else if(cnt == 1) {
            customLayout->grid = customLayout->grid2;
            customLayout->scene = customLayout->scene2;
            cnt ++;
            qDebug() << "click : 2";
        }
        else if(cnt == 2) {
            customLayout->grid = customLayout->grid3;
            customLayout->scene = customLayout->scene3;
            cnt ++;
            qDebug() << "click : 3";
        }
        else if(cnt == 3) {
            customLayout->grid = customLayout->grid4;
            customLayout->scene = customLayout->scene4;
            cnt ++;
            qDebug() << "click : 3";
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
    }

    /* view의 위치를 클릭하고 이미지를 선택한 경우 */
    else if(customLayout->g == true) {
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
    }
}


void MainWindow::layoutSizeChanged(QGraphicsView *grid)
{
    Q_UNUSED(grid);
    qDebug() << "layoutSizeChanged 슬롯이 작동합니다.";


    /* view 1,2,3,4의 크기 */
    QSize itemSize1 = customLayout->grid1->viewport()->size();
    QSize itemSize2 = customLayout->grid2->viewport()->size();
    QSize itemSize3 = customLayout->grid3->viewport()->size();
    QSize itemSize4 = customLayout->grid4->viewport()->size();

    foreach(auto item, customLayout->scene1->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize1, Qt::KeepAspectRatio));
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


//----------------------------------------------------------------------------------------------------------------

    if(d == true) {
        /* view 1,2,3,4의 크기 */
        QSize itemSize1 = customLayout->grid1->viewport()->size();
        QSize itemSize2 = customLayout->grid2->viewport()->size();
        QSize itemSize3 = customLayout->grid3->viewport()->size();
        QSize itemSize4 = customLayout->grid4->viewport()->size();

        foreach(auto item, customLayout->scene1->items()) {
            QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize1, Qt::KeepAspectRatio));
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
    }
}

void MainWindow::DoubleWidget(QGraphicsView* grid)
{
    qDebug("Double?????? Checking");
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
}

void MainWindow::previousScreen()
{
    stackWidget->setCurrentIndex(0);

    d = true;

    QSize itemSize1 = customLayout->grid1->viewport()->size();
    QSize itemSize2 = customLayout->grid2->viewport()->size();
    QSize itemSize3 = customLayout->grid3->viewport()->size();
    QSize itemSize4 = customLayout->grid4->viewport()->size();

    foreach(auto item, customLayout->scene1->items()) {
        QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize1, Qt::KeepAspectRatio));
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
}

void MainWindow::screenSizeChanged()
{
//    if(myMaxlayout->r == true) {
//        QSize itemSize5 = myMaxlayout->maxNewGrid->viewport()->size();

//        foreach(auto item, myMaxlayout->maxNewSc->items()) {
//            QGraphicsPixmapItem* pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
//            pixItem->setPixmap(QPixmap(itemHash[pixItem]).scaled(itemSize5, Qt::KeepAspectRatio));
//            myMaxlayout->maxNewSc->setSceneRect(pixItem->sceneBoundingRect());
//        }
//    }
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

void MainWindow::on_imageClearPushButton_clicked()                  // scene 이미지 초기화
{
    customLayout->grid1->scene()->clear();
    customLayout->grid2->scene()->clear();
    customLayout->grid3->scene()->clear();
    customLayout->grid4->scene()->clear();

    cnt = 0;

    customLayout->g = false;
}

/* ui 색상 및 크기 조절 */
void MainWindow::styleColor()
{
    ui->mdiArea->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter2->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter3->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter4->setStyleSheet("background:rgb(32, 56, 100)");
    ui->treeWidget1->setStyleSheet("background:rgb(255, 255, 255)");
    ui->treeWidget2->setStyleSheet("background:rgb(255, 255, 255)");
    ui->listWidget->setStyleSheet("background:rgb(255, 255, 255)");
    ui->patientTableView->setStyleSheet("background:rgb(255, 255, 255)");
    ui->doctorNameLineEdit->setStyleSheet("background:rgb(180, 199, 231)");

    /*길이 측정 및 각도 측정 레이블의 sheet색을 흰색으로 변경*/
    ui->lengthLabel->setStyleSheet(sheetWhite);
    ui->angleLabel->setStyleSheet(sheetWhite);

    /*길이 측정과 각도 측정의 라인에디트의 sheet색을 흰색으로 변경*/
    ui->lengthLineEdit->setStyleSheet(sheetWhite);
    ui->angleLineEdit->setStyleSheet(sheetWhite);
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
    ui->brightToolButton->setStyleSheet(sheetWhite);
    ui->brightnessToolButton->setStyleSheet(sheetWhite);
    ui->sharpenToolButton->setStyleSheet(sheetWhite);
    ui->contrastToolButton->setStyleSheet(sheetWhite);
    ui->horizontalFlipToolButton->setStyleSheet(sheetWhite);
    ui->verticalFlipToolButton->setStyleSheet(sheetWhite);
    ui->zoomInToolButton->setStyleSheet(sheetWhite);
    ui->zoomOutToolButton->setStyleSheet(sheetWhite);
    ui->panningToolButton->setStyleSheet(sheetWhite);
    ui->leftRotateToolButton->setStyleSheet(sheetWhite);
    ui->rightRotateToolButton->setStyleSheet(sheetWhite);

    /* Measure */
    ui->rulerToolButton->setStyleSheet(sheetWhite);
    //ui->measureToolButton->setStyleSheet(sheetWhite);
    ui->protractorToolButton->setStyleSheet(sheetWhite);
    ui->implantToolButton->setStyleSheet(sheetWhite);
    ui->implantToolButton_2->setStyleSheet(sheetWhite);
    ui->blendingButton->setStyleSheet(sheetWhite);

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
    ui->verticalSpacer_5->changeSize(12, 30);

    /* toolButton 크기 조절 */
    ui->brushToolButton->setIconSize(QSize(50,40));
    ui->brushToolButton->setText("Brush");
    ui->brushToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->colorToolButton->setIconSize(QSize(100,30));
    ui->colorToolButton->setText("Color");
    ui->colorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}


void MainWindow::customLayoutLocation()
{
    /* grid1(graphicsView) 의 좌표값 */
    int x1 = customLayout->grid1->pos().x();                 // grid1의 topLeft의 x 좌표값
    int y1 = customLayout->grid1->pos().y();                 // grid1의 topLeft의 y 좌표값
    int width1 = x1 + customLayout->grid1->width();          // grid1의 topLeft의 x좌표값에서 grid의 폭만큼 이동한 x의 좌표값
    int height1 = y1 + customLayout->grid1->height();        // grid1의 topLeft의 y 좌표값에서 grid의 높이만큼 이동한 y의 좌표값

    /* grid2(graphicsView) 의 좌표값 */
    int x2 = customLayout->grid2->pos().x();
    int y2 = customLayout->grid2->pos().y();
    int width2 = x2 + customLayout->grid2->width();
    int height2 = y2 + customLayout->grid2->height();

    /* grid3(graphicsView) 의 좌표값 */
    int x3 = customLayout->grid3->pos().x();
    int y3 = customLayout->grid3->pos().y();
    int width3 = x3 + customLayout->grid3->width();
    int height3 = y3 + customLayout->grid3->height();

    /* grid4(graphicsView) 의 좌표값 */
    int x4 = customLayout->grid4->pos().x();
    int y4 = customLayout->grid4->pos().y();
    int width4 = x4 + customLayout->grid4->width();
    int height4 = y4 + customLayout->grid4->height();

    qDebug("grid1의 TopLeft : (%d, %d)", x1, y1);
    qDebug("grid1의 TopRight : (%d, %d)", width1, y1);
    qDebug("grid1의 ButtomLeft : (%d, %d)", x1, height1);
    qDebug("grid1의 ButtomRight : (%d, %d)\n", width1, height1);

    qDebug("grid2의 TopLeft : (%d, %d)", x2, y2);
    qDebug("grid2의 TopRight : (%d, %d)", width2, y2);
    qDebug("grid2의 ButtomLeft : (%d, %d)", x2, height2);
    qDebug("grid2의 ButtomRight : (%d, %d)\n", width2, height2);

    qDebug("grid3의 TopLeft : (%d, %d)", x3, y3);
    qDebug("grid3의 TopRight : (%d, %d)", width3, y3);
    qDebug("grid3의 ButtomLeft : (%d, %d)", x3, height3);
    qDebug("grid3의 ButtomRight : (%d, %d)\n", width3, height3);

    qDebug("grid4의 TopLeft : (%d, %d)", x4, y4);
    qDebug("grid4의 TopRight : (%d, %d)", width4, y4);
    qDebug("grid4의 ButtomLeft : (%d, %d)", x4, height4);
    qDebug("grid4의 ButtomRight : (%d, %d)\n", width4, height4);
}

/* 환자 정보 DB */
void MainWindow::patientLoad()
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("patientDB.db");                             //데이터베이스 이름설정

    if(DB.open()){                                                 //조건문
        patientQuery = new QSqlQuery(DB);
        //query 문을 이용하여 테이블 생성 및 PK 키 설정
        patientQuery->exec("DROP TABLE patient");
        patientQuery->exec("CREATE TABLE IF NOT EXISTS patient(ID INTEGER Primary Key,"
                           "Name VARCHAR(20) NOT NULL, Age INTEGER, DoctorID VARCHAR(20),"
                           "PhotoDate VARCHAR(20), ImageListURL VARCHAR(100));");

        patientQueryModel = new QSqlTableModel(this, DB);
        patientQueryModel->setTable("patient");
        patientQueryModel->select();

        /*테이블 헤더 설정*/
        patientQueryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        patientQueryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        patientQueryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
        patientQueryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("DoctorID"));
        patientQueryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("PhotoDate"));
        patientQueryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("ImageListURL"));

        ui->patientTableView->setModel(patientQueryModel);
        ui->patientTableView->hideColumn(5);

        //ui->patientTableView->hideColumn(3);

        /*요청한 HTTP 경로에 JSON 데이터 파싱*/

        /*스택 위의 임시 이벤트 루프(event loop)*/
        QEventLoop eventLoop;

        /*"finished()"가 불려지면 이벤트 루프를 종료*/
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)),
                         &eventLoop, SLOT(quit()));

        QNetworkRequest req( QUrl( QString("http://" + hostName +  ':' + portNum + "/patient/") ) );
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록

        if (reply->error( ) == QNetworkReply::NoError) {
            QString strReply = (QString)reply->readAll( );
            // Json 파싱
            qDebug( ) << "Response:" << strReply;

            /*strReply변수로 모든 Json 데이터를 받으후 Json형태로 포멧팅 */
            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8( ));
            /*Json Object 변수명 설정*/
            QJsonObject jsonObject = jsonResponse.object();
            /*Json 형태의 데이터중 "patients"라는 구분자를 찾고 재배열*/
            QJsonArray jsonArr = jsonObject["patients"].toArray();

            /*"patient"구분자의 사이즈 만큼 jsonObj롤 구분*/
            for(int i = 0; i < jsonArr.size(); i++) {
                QJsonObject jsonObj = jsonArr.at(i).toObject();    //jsonResponse.object();
#if 0
                foreach(const QString& key, jsonObj.keys()) {
                    QJsonValue value = jsonObj.value(key);
                    qDebug() << "Key = " << key << ", Value = " << value.toString();
                }
#else
                /*JSON 파싱(환자 정보는 ID(int), Name(QString), Age(int), DoctorID(QString), PhotoDate(QString))*/
//                qDebug( ) << "ID:" << jsonObj["ID"].toString( );
//                qDebug( ) << "Name:" << jsonObj["Name"].toString( );
//                qDebug( ) << "Age:" << jsonObj["Age"].toInt( );
//                qDebug( ) << "DoctorID:" << jsonObj["DoctorID"].toString( );
//                qDebug( ) << "PhotoDate:" << jsonObj["PhotoDate"].toString( );
//                qDebug( ) << "ImageListURL:" << jsonObj["ImageListURL"].toString();
#endif
#if 0
                /*의사의 아이디를 구분하여 해당하는 환자의 정보를 출력*/
                QString DoctorID = "osstem2";
                if(DoctorID == jsonObj["DoctorID"].toString()){
                    /*구분된 JsonArr.size() 내부의 Json데이터를 QtDB Table에 Insert*/
                    patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES ('%1','%2',%3,'%4','%5','%6')")
                                       .arg(jsonObj["ID"].toString()).arg(jsonObj["Name"].toString())
                            .arg(jsonObj["Age"].toInt()).arg(jsonObj["DoctorID"].toString())
                            .arg(jsonObj["PhotoDate"].toString())
                            .arg(jsonObj["ImageListURL"].toString()));
                }
#else
                /*의사의 아이디를 구분하여 해당하는 환자의 정보를 출력*/
                QString DoctorID = jsonObj["DoctorID"].toString();
                if(DoctorID == "osstem1")
                {
                    /*구분된 JsonArr.size() 내부의 Json데이터를 QtDB Table에 Insert*/
                    patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES ('%1','%2',%3,'%4','%5','%6')")
                                       .arg(jsonObj["ID"].toString()).arg(jsonObj["Name"].toString())
                            .arg(jsonObj["Age"].toInt()).arg(jsonObj["DoctorID"].toString())
                            .arg(jsonObj["PhotoDate"].toString())
                            .arg(jsonObj["ImageListURL"].toString()));
                }

#
#endif
            }
            delete reply;
        }
        /*환자의 정보를 데이터 베이스 테이블에 출력*/
        patientQueryModel->select();
        /*각 환자 정보 데이터의 Column 사이즈에 맞게 resize*/
        ui->patientTableView->resizeColumnsToContents();
    }
}

/*테이블에 생성된 환자를 더블 클릭할 시 6번째(jsonObj["ImageListURL"].toString();) colum의 이미지 경로를 탐색*/
void MainWindow::on_patientTableView_doubleClicked(const QModelIndex &index)
{
    on_imageClearPushButton_clicked();

    update();
    /*더블 클릭된 환자의 이미지 리스트를 저장할 디렉터리 표시*/
    QDir dir("./Images/");
    /*모든 내용(파일)을 포함하여 해당 디렉토리(./Images/)를 제거*/
    dir.removeRecursively();
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    qDebug() << "selectDB Data Double clicked!";

    /*환자 정보 테이블 더블 클릭시 해당하는 row, column을 반환하는 변수 설정*/
    int row =  ui->patientTableView->currentIndex().row();
    int column = ui->patientTableView->currentIndex().column();

    qDebug() << row << " " << column;
    /*이미지 리스트 명을 변수로 선언 table(row, 5)에 위치한 데이터가 이미지 리스트 URL*/
    QString ImageListURLName = patientQueryModel->data(patientQueryModel->index(row, 5)).toString();
    qDebug() << ImageListURLName;

    /*환자의 아이디명을 변수로 선언 table(row, 0)에 위치한 데이터가 환자의 아이디*/
    patientID = patientQueryModel->data(patientQueryModel->index(row, 0)).toString();
    qDebug() << "patientID : " << patientID;

    /*네트워크를 통해 파일의 접근 매니저를 새로 부여 받음*/
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &MainWindow::onFinished);

    /*URL 접속 여부 확인*/

    qDebug("URL 접속 여부 확인");
    /*더블클릭된 이미지 리스트 URL안의 이미지 Json 데이터를 request*/
    request = new QNetworkRequest(QString("%1").arg(ImageListURLName));
    //    QNetworkReply *reply =
    manager->get(*request);
}

/*메니저 요청이 끝날 시 해당되는 url을 요청 받고 json데이터를 탐색한 뒤 Finished*/
void MainWindow::onFinished(QNetworkReply* reply)
{
    qDebug("MainWindow::onFinished");
    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );
        // Json 파싱
        qDebug( ) << "Response:" << strReply;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8( ));
        /*위에서 적었던 patients와 같은 구분이 없어 전체데이터를 Array화*/
        QJsonArray jsonArr = jsonResponse.array();

        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject jsonObj = jsonArr.at(i).toObject();    //jsonResponse.object();
#if 0
            foreach(const QString& key, jsonObj.keys()) {
                QJsonValue value = jsonObj.value(key);
                qDebug() << "Key = " << key << ", Value = " << value.toString();
            }
#else
            /*Json 파싱 데이터 ID(QString), ImageName(QString), PixelLength(double), ImageKinds(QString), ImagePathURL(QString)*/
            //qDebug( ) << "ID:" << jsonObj["ID"].toString( );
            //qDebug( ) << "PixelLength:" << jsonObj["PixelLength"].toDouble( );
            //qDebug( ) << "ImageKinds:" << jsonObj["ImageKinds"].toString();
            /*이게 제일 중요 해당 이미지를 이 URL에서 호출하도록 해야함*/
            qDebug( ) << "Imagename: " << jsonObj["ImageName"].toString( );
            qDebug( ) << "ImagePathURL:" << jsonObj["ImagePathURL"].toString(); //이미지 URL 호출

            /* 데이터 베이스 테이블을 클릭시 해당하는 URL의 ImageURL */
            QString ImageURL = jsonObj["ImagePathURL"].toString();
            QString ImageName = jsonObj["ImageName"].toString( );

            qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
#endif
            //patientID = jsonObj["ID"].toString();
            /*이미지를 다운로드 받을 수 있는 변수 재할당*/
            downLoader = new Downloader(this);

            /*이미지 URL, 다운로드 받을 폴더 명, 이미지 파일 이름*/
            /*환자 ID별로 이미지 디렉토리를 생성*/
            downLoader->setFile(ImageURL, "./Images/", ImageName);
            //downLoader->setFile(ImageURL, QString("./SaveImage%1").arg(patientID), ImageName);

            /*업로드 완료 시그널후 메인 윈도우의 listWidget에서 이미지를 업로드 받는 슬롯*/
            connect(downLoader, &Downloader::sendUpload, this, &MainWindow::receiveupload);
            qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
        }
        reply->deleteLater();
    }
}

/*파일과 디렉토리를 복사하는 함수 srcDir : 복사할 디렉토리, copyDir : 복사받을 디렉토리*/
void MainWindow::copyPath(QString srcDir, QString copyDir)
{
    /*복사할 디렉토리를 dir변수로 명시*/
    QDir dir(srcDir);

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

/*Downloader 클래스의 upload가 끝날 시 이미지를 로드 하는 슬롯*/
void MainWindow::receiveupload()
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    loadImages();
}

/*Downloader클래스의 복사 생성자 내부의 manager 변수 new 할당*/
Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
}

Downloader::~Downloader()
{
    manager->deleteLater();
}

/*MainWindow의 downloader클래스의 setFile함수 파라미터 변수들을 토대로 file저장*/
void Downloader::setFile(QString fileURL, QString folderName, QString fileName)
{
    /*서버로 부터 받은 이미지 파일을 다운로드 하기위한 디렉토리 생성*/
    QDir dir;
    /*해당 디렉토리가 존재하는 경우 그대로 유지 /
      디렉토리가 없는 경우 folderName 그대로 새 디렉토리를 생성*/
    if(dir.exists(folderName)){
        qDebug() << "Existis: " + folderName;
    }else{
        /*mkdir = make directory*/
        dir.mkdir(folderName);
        qDebug() << "Created: " + folderName;
    }

    /*서버로 부터 받을 이미지 경로와 내부 파일 데이터를 png형태로 포멧*/
    QString filePath = fileURL;
    QStringList filePathList = filePath.split('/');
    QString fileExt = filePathList.at(filePathList.count() - 1);
    fileExt = "png";
    QString saveFilePath;
    saveFilePath = QString(folderName + "/" + fileName + "." + fileExt );

    /*요청받은 URL의 파일을 응답*/
    QNetworkRequest request;
    request.setUrl(QUrl(fileURL));
    reply = manager->get(request);

    /*서버로부터 받은 데이터를 파일로 전환*/
    file = new QFile;
    file->setFileName(saveFilePath);

    /*요청에 따른 서버 내 이미지 파일 다운로드와 완료여부를 확인하는 connect함수*/
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
}

/*다운로드 진행과정을 프로그래스 형태로 출력*/
void Downloader::onDownloadProgress(qint64 bytesRead,qint64 bytesTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() +" - "+ QString::number(bytesTotal).toLatin1());
}

/*다운로드 완료여부 출력 후 메인윈도우 listWidget 업로드*/
void Downloader::onFinished(QNetworkReply * reply)
{
    /*파일을 먼저 닫은 다음에 emit신호를 보내야 전체 이미지 리스트들을 부를 수 있음*/
    /*close() -> relpy()*/
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }

    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    switch(reply->error())
    {
    /*네트워크 요청에 에러사항이 없는 경우*/
    case QNetworkReply::NoError:
    {
        qDebug("file is downloaded successfully.");

        emit sendUpload();
        qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    }break;
    default:{
        qDebug(reply->errorString().toLatin1());
    };
    }
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
}

/*다운받는 파일을 기다리다가 파일을 WriteOnly형식으로 모두 읽어오기*/
void Downloader::onReadyRead()
{
    qDebug() << "Ready";
    file->open(QIODevice::WriteOnly);
    file->write(reply->readAll());
}

/*요청이 모두 끝난 경우 파일을 닫고 소멸*/
void Downloader::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}

/*Blending Button 클릭시 해당 다이얼로그 호출*/
void MainWindow::on_blendingButton_clicked()
{
    QDir dir("./Images/");
    if(dir.isEmpty()){
        QMessageBox::information(this, "Blending Dialog",
                                 "Do Not any show listWidget");
        return;
    }
    blendDialog = new Blending(this);
    //blendDialog->setWindowFlag(WindowTitleHint | WindowMinimizeButtonHint | WindowType::WindowMaximizeButtonHint))
    blendDialog->exec();
}

/*Load 버튼 클릭시 해당환자의 저장 이미지와 서버이미지를 같이 업로드*/
void MainWindow::on_actionload_triggered()
{
    /*저장했던 이미지 폴더를 불러서 환자정보 이미지와 저장한 이미지를 동시에 출력할 수 있도록 조정*/
    copyPath(QString("./SaveImage%1").arg(patientID), "./Images");
    QDir dir("./Images");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::NoSort);

    auto entryList = dir.entryList();

    QCollator collator;
    collator.setNumericMode(true);

    std::sort(
        entryList.begin(),
        entryList.end(),
        [&](const QString &file1, const QString &file2)
        {
            return collator.compare(file1, file2) > 0;
        });

    loadImages();
}

