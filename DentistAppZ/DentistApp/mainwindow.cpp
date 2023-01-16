#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
#include "fmx.h"

/* 환자 정보 DB */
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>
//#include <opencv2/opencv.hpp>

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

    /*버튼 클릭시 해당 gridLayout에 기능 적용*/
    connect(this,SIGNAL(requestBrightUp()),gridLayout,SLOT(brightUp()));
    connect(this,SIGNAL(requestBrightLow()),gridLayout,SLOT(brightnessLow()));
    connect(this,SIGNAL(requestInverstion()),gridLayout,SLOT(inverstion()));
    connect(this,SIGNAL(requestProtractor()),gridLayout,SLOT(proTractor()));
    connect(this,SIGNAL(requestRuler()),gridLayout,SLOT(ruler()));
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

    /*빌드 폴더 내에 png 데이터 전체 삭제*/
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
                //qDebug() << "Response:" << strReply;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
                QJsonObject jsonObj = jsonResponse.object();

                //qDebug() << "id : " << jsonObj["id"].toInt();
                //qDebug() << "name : " << jsonObj["name"].toString();
                //qDebug() << "age : " << jsonObj["age"].toInt();
                //qDebug() << "local : " << jsonObj["local"].toString();
                patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES (%1,'%2',%3,'%4')")
                                   .arg(jsonObj["id"].toInt()).arg(jsonObj["name"].toString())
                        .arg(jsonObj["age"].toInt()).arg(jsonObj["local"].toString()));
                delete reply;
            }else{  //오류시
                //qDebug() << "Failure" << reply->errorString();
                delete reply;
            }
        }
        patientQueryModel->select();
        ui->patientTableView->resizeColumnsToContents();

    }
}


void MainWindow::on_brightToolButton_clicked()
{
    emit requestBrightUp();
}


void MainWindow::on_brightnessToolButton_clicked()
{
    emit requestBrightLow();
}


void MainWindow::on_inversionToolButton_clicked()
{
    emit requestInverstion();
}

void MainWindow::on_protractorToolButton_clicked()
{
    emit requestProtractor();
}

void MainWindow::on_rulerToolButton_clicked()
{
    emit requestRuler();
}

/*복사된 이미지를 만들때 1로 초기화 하여 copy(1~N).png를 생성*/
int num = 1;
void MainWindow::on_patientTableView_doubleClicked(const QModelIndex &index)
{
    //qDebug() << "selectDB Data Double clicked!";

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
            //qDebug() << n1 << " " << n2 << " " << n3 << " " << n4;
            /*선택된 ID의 다운로드 경로에서 이미지를 Qt클라이언트 build 파일로 업로드*/

            /*서버에서 다운로드된 이미지의 경로에서 빌드폴더로 copy(1~4).png 명으로 복사*/
            QFile::copy(n4, QString("./copy%1.png").arg(num));
            //qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
            loadImages();
        }
    }
    num++;

    qDebug() << row << " " << column;
}



