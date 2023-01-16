#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
#include "fmx.h"
//#include "listalbum.h"

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

#include <QGraphicsScene>
#include <QGraphicsView>

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

/*QMessageBox 호출*/
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));

    loadImages();


    QList<int> sizes;
    sizes << 700 << 70;
    ui->splitter->setSizes(sizes);

    QList<int> sizes2;
    sizes2 << 500 << 130;
    ui->splitter2->setSizes(sizes2);

    QList<int> sizes3;
    sizes3 << 100 << 550;
    ui->splitter3->setSizes(sizes3);

    ui->verticalSpacer->changeSize(20, 15);

    /* 버튼 크기 조절 */
    createToolButton();

    /* 환자 정보 DB */
    patientLoad();

    /* WindowTitle 지정 */
    /* 레이아웃 */
    customLayout = new Layout(this);
    customLayout->setWindowTitle(tr("2 X 2 Grid"));
    connect(customLayout, SIGNAL(destroyed()), customLayout, SLOT(deleteLater()));

    fmx = new FMX(this);
    fmx->setWindowTitle(tr("14 FMX"));
    connect(fmx, SIGNAL(destroyed()), fmx, SLOT(deleteLater()));

/*------------------------------------------------------------------------------------------------------------------------ 시그널 슬롯 */

    /* 이미지 클릭 시 시그널 발생, listAlbum에서 item 선택 시 슬롯 동작 */


/*------------------------------------------------------------------------------------------------------------------------------------*/

    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(customLayout);
    ui->mdiArea->addSubWindow(fmx);
    ui->mdiArea->setActiveSubWindow(cw);
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

    //빌드 폴더 내에 png 데이터 전체 삭제
    for(int i = 0; i < 1000; i++){
        QFile::remove(QString("./copy%1.png").arg(i));
    }
}

void MainWindow::loadImages()
{
    QDir dir(".");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    //gridLayout->scene->clear();

    ui->listWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, ui->listWidget);
        item->setStatusTip(fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
}

void MainWindow::selectItem(QListWidgetItem *item){
//    //qDebug("%d %d", customLayout->grid1->scene()->width(), customLayout->grid1->scene()->height());
//    customLayout->grid1->resetTransform();
////    customLayout->scene1->sceneRect().isEmpty();
////    customLayout->scene1->clear();
////    customLayout->scene2->clear();
////    customLayout->scene3->clear();
////    customLayout->scene4->clear();
//    //gridLayout->scene->addPixmap(QPixmap(item->statusTip()).scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//    customLayout->scene1->addPixmap(QPixmap(item->statusTip()).scaledToHeight(customLayout->grid1->height()-2));
//    customLayout->scene2->addPixmap(QPixmap(item->statusTip()).scaledToHeight(customLayout->grid2->height()-2));
//    customLayout->scene3->addPixmap(QPixmap(item->statusTip()).scaledToHeight(customLayout->grid3->height()-2));
//    customLayout->scene4->addPixmap(QPixmap(item->statusTip()).scaledToHeight(customLayout->grid4->height()-2));
//    customLayout->grid1->setAlignment(Qt::AlignCenter);
//    customLayout->grid2->setAlignment(Qt::AlignCenter);
//    customLayout->grid3->setAlignment(Qt::AlignCenter);
//    customLayout->grid4->setAlignment(Qt::AlignCenter);

////    qDebug("%d", customLayout->scene->height());


    static QGraphicsScene *beforeScene = NULL;              // static이 붙으면 지역변수여도 클래스 안에서 모두 쓸 수 있음.
    if(beforeScene != NULL)
        delete beforeScene;

    QGraphicsScene *newScene = new QGraphicsScene;
    beforeScene = newScene;
    newScene->setBackgroundBrush(Qt::black);

    customLayout->grid1->setScene(newScene);
    newScene->addPixmap(QPixmap(item->statusTip()).scaledToHeight(customLayout->grid1->height()-2));

    customLayout->grid1->setAlignment(Qt::AlignCenter);
}






void MainWindow::patientLoad()
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
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

        /*요청한 HTTP 경로에 JSON 데이터 파싱*/

        /*스택 위의 임시 이벤트 루프(event loop)*/
        QEventLoop eventLoop;

        /*"finished()"가 불려지면 이벤트 루프를 종료*/
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)),
                         &eventLoop, SLOT(quit()));

        /*HTTP 요청(1-4)*/
        for(int patientNum = 1; patientNum <= 4; patientNum++){
            QNetworkRequest req(QUrl(QString("http://127.0.0.1:3000/patient/%1/info").arg(patientNum)));
            QNetworkReply *reply = mgr.get(req);
            eventLoop.exec();       //"finished()" 가 호출 될 때까지 블록

            /*요청에 이상이 없는 경우*/
            if(reply->error() == QNetworkReply::NoError){
                QString strReply = (QString)reply->readAll();

                //JSON 파싱
                qDebug() << "Response:" << strReply;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
                QJsonObject jsonObj = jsonResponse.object();

                qDebug() << "id : " << jsonObj["id"].toInt();
                qDebug() << "name : " << jsonObj["name"].toString();
                qDebug() << "age : " << jsonObj["age"].toInt();
                qDebug() << "local : " << jsonObj["local"].toString();
                patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES (%1,'%2',%3,'%4')")
                                   .arg(jsonObj["id"].toInt()).arg(jsonObj["name"].toString())
                                   .arg(jsonObj["age"].toInt()).arg(jsonObj["local"].toString()));
                delete reply;
            }else{  //오류시
                qDebug() << "Failure" << reply->errorString();
                delete reply;
            }
        }

        patientQueryModel->select();
        ui->patientTableView->resizeColumnsToContents();
    }
}

void MainWindow::createToolButton()
{
    ui->BrushtoolButton->setIconSize(QSize(50,40));
    ui->BrushtoolButton->setText("Brush");
    ui->BrushtoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->ColortoolButton->setIconSize(QSize(100,30));
    ui->ColortoolButton->setText("Color");
    ui->ColortoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

/*복사된 이미지를 만들때 1로 초기화 하여 copy(1~N).png를 생성*/
int num = 1;
void MainWindow::on_patientTableView_doubleClicked(const QModelIndex &index)
{
    qDebug() << "selectDB Data Double clicked!";

    int row =  ui->patientTableView->currentIndex().row();
    int column = ui->patientTableView->currentIndex().column();

    /*데이터 베이스 모델의 1~4 row Data*/
    for(int i = 0; i < patientQueryModel->rowCount(); i++){
        /*선택된 데이터 베이스 모델 row 테이블의 데이터를 추출*/
        int n1 = patientQueryModel->data(patientQueryModel->index(i, 0)).toInt();               //ID
        QString n2 = patientQueryModel->data(patientQueryModel->index(i, 1)).toString();        //이름
        int n3 = patientQueryModel->data(patientQueryModel->index(i, 2)).toInt();               //나이
        QString n4 = patientQueryModel->data(patientQueryModel->index(i, 3)).toString();        //서버에서 다운받은 이미지 폴더 경로

        /*선택된 row + 1과 DB ID로 저장된 번호가 일치하면 (row + 1 = ID)*/
        /*서버에서 다운로드 받은 이미지 폴더를 빌드폴더로 이동*/
        if(n1 == row + 1){
            qDebug() << n1 << " " << n2 << " " << n3 << " " << n4;
            /*선택된 ID의 다운로드 경로에서 이미지를 Qt클라이언트 build 파일로 업로드*/

            /*서버에서 다운로드된 이미지의 경로에서 빌드폴더로 copy(1~4).png 명으로 복사*/
            QFile::copy(n4, QString("./copy%1.png").arg(num));
            qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
            loadImages();
        }
    }
    num++;

    qDebug() << row << " " << column;
}


//int orthoNum = 0;
//int xPos1 = 0;
//int yPos1 = 0;
//int xPos2 = 0;
//int yPos2 = 0;
//void MainWindow::OrthoReceive(int _x, int _y){
//    update();
//    if(!selectAction->isChecked()){
//        qDebug() << "OrthoReceive : " << _x << ", " << _y;
//        qDebug() << "check : " << orthoNum;
//        if(num == 0){
//            xPos1 = _x;
//            yPos1 = _y;
//        }
//        if(num == 1){
//            xPos2 = _x;
//            yPos2 = _y;
//            qDebug() << "x1 = " << xPos1 <<
//                        "y1 = " << yPos1 <<
//                        "x2 = " << xPos2 <<
//                        "y2 = " << yPos2;

//            //삼각함수 기법을 활용한 line의 길이 측정
//            int z = qSqrt(qPow((xPos2 - xPos1), 2) + qPow((yPos2 - yPos1), 2));
//            qDebug() << "z : " << z;
//            //text = scene->addText(QString("%1").arg(z));
//            //text->setPos(x_2, y_2);
//            QMessageBox::information(this,"length",
//                                    QString("length info : %1").arg(z / 20));
//            //사진의 축적 비율 정보만 알면 실제 길이를 측정 할 수 있을 것 같음
//        }

//        orthoNum++;
//        if(orthoNum > 1){
//            selectAction->setCheckable(true);
//            selectAction->setChecked(true);
//            lineAction->setChecked(false);
//            scene->setMode(Scene::SelectObject);
//            //text->setEnabled(false);
//            //delete text;
//            orthoNum = 0;
//        }
//    }else{
//        return;
//    }
//}

//void MainWindow::on_RulertoolButton_triggered(QAction *arg1)
//{
//    scene->setMode(Scene::Mode(arg1->data().toInt()));
//}

