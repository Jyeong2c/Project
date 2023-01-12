#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include<QToolButton>

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->BrushtoolButton->setIcon(QIcon("draw.png"));
    ui->BrushtoolButton->setIconSize(QSize(30,100));
    ui->BrushtoolButton->setText("Draw Nerve");
    ui->BrushtoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    ui->ColortoolButton->setIcon(QIcon("color.png"));
    ui->ColortoolButton->setIconSize(QSize(30,100));
    ui->ColortoolButton->setText("Color");
    ui->ColortoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    patientLoad();
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

void MainWindow::patientLoad()                                   //파일 저장 함수
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

        /*HTTP 요청(1-2)*/
        for(int patientNum = 1; patientNum <= 2; patientNum++){
            QNetworkRequest req(QUrl(QString("http://127.0.0.1:3000/patient/%1/").arg(patientNum)));
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
                qDebug() << "localimage : " << jsonObj["localimage"].toString();
                qDebug() << "Failure" << reply->errorString();
                patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES (%1,'%2','%3','%4')")
                                   .arg(jsonObj["id"].toInt()).arg(jsonObj["name"].toString())
                                   .arg(jsonObj["age"].toInt()).arg(jsonObj["localimage"].toString()));
                delete reply;
            }else{  //오류시
                qDebug() << "Failure" << reply->errorString();
                delete reply;
            }
        }

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
