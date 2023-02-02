#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
#include "maxlayout.h"
#include "keyfeaturesform.h"
#include "scene.h"
#include "downloader.h"

/* 환자 정보 DB */
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>

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
#include <QEventLoop>

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

    keyFeatures = new KeyFeaturesForm();                                              // 주요 기능 클래스
    connect(keyFeatures, SIGNAL(destroyed()), keyFeatures, SLOT(deleteLater()));

    //scene = new Scene;


    customLayout = new Layout(this);                                                // 레이아웃 화면
    connect(customLayout, SIGNAL(destroyed()), customLayout, SLOT(deleteLater()));
    myMaxlayout = new Maxlayout(this);                                              // 최대 창
    connect(myMaxlayout, SIGNAL(destroyed()), myMaxlayout, SLOT(deleteLater()));


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
    //loadImages();                                   // listWidget에 이미지 로드

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    manager = new QNetworkAccessManager(this);
    request.setUrl(QUrl( QString("http://" + hostName +  ':' + portNum + "/patient/")));

    /* 시그널 슬롯은 위치가 중요 동적할당(new)보다 밑에 있을 것 */
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));
    connect(customLayout, SIGNAL(sig_size(QGraphicsView*)), SLOT(layoutSizeChanged(QGraphicsView*)));
    connect(myMaxlayout, SIGNAL(max_sig_size(QGraphicsView*)), SLOT(DoubleWidget(QGraphicsView*)));
    connect(customLayout, SIGNAL(sig_widgetbyDClick(QGraphicsView*)), SLOT(DoubleWidget(QGraphicsView*)));
    connect(myMaxlayout->viewQuit, SIGNAL(clicked()), SLOT(previousScreen()));

    connect(manager, SIGNAL(finished(QNetworkReply*)), &connectionLoop, SLOT(quit()));

    /*길이 측정 좌표 signal과 좌표값을 받는 슬롯*/
    connect(customLayout->scene1, &Scene::sendMeasureLength, this, &MainWindow::receiveLengthMeasure);
    /*각도 측정 결과 signal과 결과값을 받는 슬롯*/
    connect(customLayout->scene1, &Scene::sendMeasureAngle, this, &MainWindow::receiveAngleMeasure);
    /*각 Scene마다 픽셀당 실길이를 보내는 커넥트 함수*/
    connect(this, &MainWindow::sendImageInfo, customLayout->scene1, &Scene::reImageInfo);
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
    /* view의 위치를 선택하지 않고 listWidget에 있는 이미지를 먼저 선택한 경우 */
    if(customLayout->g == false) {
        if(cnt == 0) {
            customLayout->grid = customLayout->grid1;
            customLayout->scene = customLayout->scene1;
            cnt ++;
        }
        else if(cnt == 1) {
            customLayout->grid = customLayout->grid2;
            customLayout->scene = customLayout->scene2;
            cnt ++;
        }
        else if(cnt == 2) {
            customLayout->grid = customLayout->grid3;
            customLayout->scene = customLayout->scene3;
            cnt ++;
        }
        else if(cnt == 3) {
            customLayout->grid = customLayout->grid4;
            customLayout->scene = customLayout->scene4;
            cnt ++;
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

        customLayout->grid->resetTransform();
    }
}


void MainWindow::layoutSizeChanged(QGraphicsView *grid)
{
    Q_UNUSED(grid);

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
}

void MainWindow::on_layoutClearPushButton_clicked()                    // scene 이미지 초기화
{
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

        reply = manager->get(request);
        connectionLoop.exec( );           // 동기화를 위한 이벤트 루프
        QByteArray data = reply->readAll();
        if (reply->error( ) == QNetworkReply::NoError) {
            //QString strReply = (QString)reply->readAll();
            QJsonDocument doc1 = QJsonDocument::fromJson(data);
            if(doc1.isObject() == false) qDebug() << "It is not a Json object";
            QJsonArray jsonArr = doc1["patients"].toArray();

            for(int i = 0; i < jsonArr.size(); i++){
                QJsonObject jsonObj = jsonArr.at(i).toObject();
                QJsonObject patientObj = jsonObj.constFind("patient")->toObject();
                QString ID = patientObj["ID"].toString();
                QString Name = patientObj["Name"].toString();
                int Age = patientObj["Age"].toInt();
                QString DoctorID = patientObj["DoctorID"].toString();
                QString PhotoDate = patientObj["PhotoDate"].toString();
                QString ImageListURL = patientObj["ImageListURL"].toString();

                /*의사의 아이디를 구분하여 해당하는 환자의 정보를 출력*/
                qDebug() << ui->doctorNameLineEdit->text();
                /*로그인 아이디와 doctorNameLineEdit의 데이터가 일치하는 경우*/
                if(DoctorID == ui->doctorNameLineEdit->text())
                {
                    /*구분된 JsonArr.size() 내부의 Json데이터를 QtDB Table에 Insert*/
                    patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES ('%1','%2',%3,'%4','%5','%6')")
                                       .arg(patientObj["ID"].toString()).arg(patientObj["Name"].toString())
                            .arg(patientObj["Age"].toInt()).arg(patientObj["DoctorID"].toString())
                            .arg(patientObj["PhotoDate"].toString())
                            .arg(patientObj["ImageListURL"].toString()));
                }
            }


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
    update();
    customLayout->grid1->scene()->clear();
    customLayout->grid2->scene()->clear();
    customLayout->grid3->scene()->clear();
    customLayout->grid4->scene()->clear();
    cnt = 0;
    customLayout->g = false;

    /*네트워크를 통해 파일의 접근 매니저를 새로 부여 받음*/
    manager = new QNetworkAccessManager(this);
    /*네트워크메니저로 서버의 환자당 이미지 리스트 받기 위한 connect함수*/
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onFinished);


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

    /*URL 접속 여부 확인*/

    qDebug("URL 접속 여부 확인");
    /*더블클릭된 이미지 리스트 URL안의 이미지 Json 데이터를 request*/
    request.setUrl(QUrl(QString("%1").arg(ImageListURLName)));
    //    QNetworkReply *reply =
    manager->get(request);
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

        /*서버의 환자 1명당 15장의 이미지를 관리하기 위한 폴더*/
        QDir dir("./ImageInfo/");
        /*이전 환자 15장의 이미지 폴더를 제거*/
        dir.removeRecursively();
        /*서버의 환자 1명당 15장의 이미지정보를 저장하기 위한 .ini 파일을 imageCount로 생성*/
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject jsonObj = jsonArr.at(i).toObject();    //jsonResponse.object();

            /*Json 파싱 데이터 ID(QString), ImageName(QString), PixelLength(double), ImageKinds(QString), ImagePathURL(QString)*/
            /*이게 제일 중요 해당 이미지를 이 URL에서 호출하도록 해야함*/
            qDebug( ) << "Imagename: " << jsonObj["ImageName"].toString( );
            qDebug( ) << "ImagePathURL:" << jsonObj["ImagePathURL"].toString(); //이미지 URL 호출
            QString ImageID = jsonObj["ImageName"].toString();
            double ImagePixelLength = jsonObj["PixelLength"].toDouble();
            QString ImageKinds = jsonObj["ImageKinds"].toString();
            /* 데이터 베이스 테이블을 클릭시 해당하는 URL의 ImageURL */
            QString ImageURL = jsonObj["ImagePathURL"].toString();
            QString ImageName = jsonObj["ImageName"].toString( );
            int ImageWidth = jsonObj["ImageWidth"].toInt();
            int ImageHeight = jsonObj["ImageHeight"].toInt();

            /*ini파일을 생성하기 위한 ImageInfo 디렉토리가 없으면 현재 디렉토리에 ImageInfo 디렉토리 생성*/
            if(!dir.exists()) dir.mkdir("./ImageInfo/");

            /*ImageInfo 디렉토리 내부에 ImageName.ini 파일을 생성*/
            QSettings ImageInfo(QString("./ImageInfo/%1.ini").arg(ImageName),
                                QSettings::IniFormat);

            //            std::stringstream ss;
            //            ss << setprecision(6) << ImagePixelLength;

            /*ini파일의 시작 그룹을 ImageInfo로 시작*/
            ImageInfo.beginGroup("ImageInfo");
            ImageInfo.setValue("Image_ID", ImageID);
            ImageInfo.setValue("Image_Pixel_Length", ImagePixelLength);
            ImageInfo.setValue("Image_Kinds", ImageKinds);
            ImageInfo.setValue("Image_URL", ImageURL);
            ImageInfo.setValue("Image_Name", ImageName);
            ImageInfo.setValue("Image_Width", ImageWidth);
            ImageInfo.setValue("Image_Height", ImageHeight);
            /*ini파일의 정보를 다 받으면 endGroup하여 이미지 정보를 ini파일 형태로 저장*/
            ImageInfo.endGroup();

            qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
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



/*Blending Button 클릭시 해당 다이얼로그 호출*/
void MainWindow::on_blendingButton_clicked()
{
    QDir dir("./Images/");
    /*./Image 디렉토리가 없을경우*/
    if(dir.isEmpty()){
        QMessageBox::information(this, "Blending Dialog",
                                 "Do Not any show listWidget");
        /*./Image 디렉토리의 내용이 없다고 알리며 예외처리*/
        return;
    }else{
        /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
        QMessageBox::StandardButton buttonReply;

        /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
        buttonReply = QMessageBox::question(this, "question", "Are you Open Blending Dialog?",
                                            QMessageBox::Yes | QMessageBox::No);

        /*버튼을 누를 시 Yes인 경우*/
        if(buttonReply == QMessageBox::Yes){
            /*blendDialog 호출*/
            blendDialog = new Blending(this);
            blendDialog->exec();
        } else {
            /*호출하지 않음*/
            return;
        }
    }
}

/*각도 측정 결과값을 출력하는 슬롯 함수 구현부*/
void MainWindow::receiveAngleMeasure(double angle)
{
    /*길이 측정 슬롯과 마찬 가지로 Scene 상에 각도 결과를 나타낼 수 없으면 라인에디트나 메세지박스로 출력*/
    QMessageBox::information(this, "Angle Result",
                             QString("Angle : %1").arg(angle));
    ui->angleLineEdit->setText(QString("%1º").arg(angle));
}

/*각도 측정 결과값을 출력하는 슬롯 함수 구현부*/
void MainWindow::receiveLengthMeasure(double length)
{
    /*길이 측정 슬롯과 마찬 가지로 Scene 상에 각도 결과를 나타낼 수 없으면 라인에디트나 메세지박스로 출력*/
    QMessageBox::information(this, "Length Result",
                             QString("Length : %1").arg(length));
    ui->lengthLineEdit->setText(QString("%1 cm").arg(length));
}

void MainWindow::on_actionload_triggered()
{
    /*저장했던 이미지 폴더를 불러서 환자정보 이미지와 저장한 이미지를 동시에 출력할 수 있도록 조정*/
    copyPath(QString("./SaveImage%1").arg(patientID), "./Images");
    QDir dir("./Images");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::NoSort);

    loadImages();
}

/*길이 측정 버튼을 클릭 시 길이 측정 기능 활성화*/
void MainWindow::on_rulerToolButton_clicked()
{
    customLayout->scene1->setCurrentShape(Scene::Length);
}

/*각도 측정 버튼을 클릭 시 각도 측정 기능 활성화*/
void MainWindow::on_protractorToolButton_clicked()
{
    customLayout->scene1->setCurrentShape(Scene::Angle);
}

/*환자 정보를 클릭 시 해당 이미지와 맞는 png파일이 있는지를 확인후 해당 png파일을 ini파일과 싱크*/
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    /*listWidget 내의 이미지의 row(), column() 데이터를 출력(listWidget은 세로 1줄만 출력되므로 row() 데이터 밖에 나오지 않음)*/
    qDebug() << "index.row() : " << index.row() << " , " << "index.colum() : " << index.column();
    /*./Images의 디렉토리의 이미지 데이터를 찾음*/
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    /*파일정보 리스트에 png, jpg, bmp파일들만 리스트정보에 넣어둠*/
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    /*png 파일을 baseName으로 */
    QString iniInfo = fileInfoList.at(index.row()).baseName() + ".ini";

    /*이미지 정보가 들어있는 디렉토리 내부의 ini파일 불러오기*/
    QFile imgInfoFile(QString("./ImageInfo/%1").arg(iniInfo));
    /*ini파일 정보가 있다면 존재여부 메세지 박스 출력*/
    if(imgInfoFile.exists()){
        QSettings ImageInfo(QString("./ImageInfo/%1").arg(iniInfo), QSettings::IniFormat);
        /*double 값이 나오기 위해 beginGroup을 설정해야 함..(setvalue할 때도 group으로 시작하고 group으로 끝냄)*/
        ImageInfo.beginGroup("ImageInfo");
        /*픽셀당 이미지의 길이, 크기 정보 변수 설정*/
        double imagePixelLength = ImageInfo.value("Image_Pixel_Length").toDouble();
        int imageWidth = ImageInfo.value("Image_Width").toInt();
        int imageHeight = ImageInfo.value("Image_Height").toInt();
        ImageInfo.endGroup();
        qDebug() << "image pixel : " << imagePixelLength;
        QMessageBox::information(this, "Exist", "exist : " + iniInfo + "\n"
                                 + QString("and PixelData : %1").arg(imagePixelLength) + "\n"
                                 + QString("ImageWidth : %1").arg(imageWidth) + "\n"
                                 + QString("ImageHeight : %1").arg(imageHeight));
        /*ini파일에서 저장된 정보(픽셀당 이미지 길이, 크기 정보)를 추출후 signal 설정*/
        emit sendImageInfo(imagePixelLength, imageWidth, imageHeight);
    } else {
        QMessageBox::information(this, "Not Exist", "not exist : " + iniInfo);
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

    /*길이 측정 및 각도 측정 레이블의 sheet색을 흰색으로 변경*/
    ui->lengthLabel->setStyleSheet("color:rgb(255, 255, 255)");
    ui->angleLabel->setStyleSheet("color:rgb(255, 255, 255)");

    /*길이 측정과 각도 측정의 라인에디트의 sheet색을 흰색으로 변경*/
    ui->lengthLineEdit->setStyleSheet(sheetWhite);
    ui->lengthLineEdit->setReadOnly(true);
    ui->angleLineEdit->setStyleSheet(sheetWhite);
    ui->angleLineEdit->setReadOnly(true);

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
    ui->protractorToolButton->setStyleSheet(sheetWhite);
    ui->implantToolButton->setStyleSheet(sheetWhite);
    ui->implantToolButton_2->setStyleSheet(sheetWhite);
    ui->blendingButton->setStyleSheet(sheetWhite);
    ui->measureClear->setStyleSheet(sheetWhite);


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
    ui->imageSizeClearPushButton->setFixedSize(70, 40);


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
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }

    myMaxlayout->maxNewGrid->scale(1.25, 1.25);
    customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void MainWindow::on_zoomOutToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->scale(0.8, 0.8);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    myMaxlayout->maxNewGrid->scale(0.8, 0.8);
    customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void MainWindow::on_leftRotateToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->rotate(90);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    myMaxlayout->maxNewGrid->rotate(90);
    customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void MainWindow::on_rightRotateToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->grid->rotate(-90);
        customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    myMaxlayout->maxNewGrid->rotate(-90);
    customLayout->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    customLayout->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void MainWindow::on_imageSizeClearPushButton_clicked()
{
    if(customLayout->g == true)
        customLayout->grid->resetTransform();

    myMaxlayout->maxNewGrid->resetTransform();
}

/*의사 아이디 정보가 넘어가는 동시에 환자정보리스트 나열*/
void MainWindow::slot_doctorInfo(QString DoctorID)
{
    qDebug() << "의사 ID를 받음";
    ui->doctorNameLineEdit->setText(DoctorID);
    /*환자 정보를 부르기 전에 로그인 플래그를 걸어줌*/
    patientLoad();                                  // 환자 정보 DB 함수

    emit sig_doctorInfo(DoctorID);
    qDebug() << "의사 ID 시그널";
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->mdiArea->mapToGlobal(pos);
    //    menu->exec(globalPos);
}
