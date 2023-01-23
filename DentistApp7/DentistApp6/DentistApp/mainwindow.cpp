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

/*해당되는 URL로부터 이미지로드를 위한 헤더*/
#include <iostream>
#include <windows.h>
#include <vector>
#include <QProgressDialog>
#include <QWebSocketServer>
#include <QMessageBox>
#include <QWebSocket>
#include <QDataStream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) /*, byteReceived(0), totalSize(0)*/
{
    ui->setupUi(this);

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));

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
}

/*서버의 환자를 클릭시 ./Images/ 에 위치한 이미지들을 listWidget에 불러오는 함수*/
void MainWindow::loadImages()
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    update();
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


void MainWindow::selectItem(QListWidgetItem *item){
    static QGraphicsScene *beforeScene = NULL;              // static이 붙으면 지역변수여도 클래스 안에서 모두 쓸 수 있음.
    if(beforeScene != NULL)
        delete beforeScene;

    QGraphicsScene *newScene = new QGraphicsScene;
    beforeScene = newScene;
    newScene->setBackgroundBrush(Qt::black);

    //    customLayout->grid1->setScene(newScene);
    //    newScene->addPixmap(QPixmap(item->statusTip()).scaledToHeight(customLayout->grid1->height()-2));

    //    customLayout->grid1->setAlignment(Qt::AlignCenter);

    customLayout->scene1->clear();
    customLayout->grid1->setScene(customLayout->scene1);
    QSize size = customLayout->grid1->viewport()->size();
    customLayout->scene1->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio));
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
                //JSON 파싱(환자 정보는 ID(int), Name(QString), Age(int), DoctorID(QString), PhotoDate(QString))
                qDebug( ) << "ID:" << jsonObj["ID"].toString( );
                qDebug( ) << "Name:" << jsonObj["Name"].toString( );
                qDebug( ) << "Age:" << jsonObj["Age"].toInt( );
                qDebug( ) << "DoctorID:" << jsonObj["DoctorID"].toString( );
                qDebug( ) << "PhotoDate:" << jsonObj["PhotoDate"].toString( );
                qDebug( ) << "ImageListURL:" << jsonObj["ImageListURL"].toString();

                /*구분된 JsonArr.size() 내부의 Json데이터를 QtDB Table에 Insert*/
                patientQuery->exec(QString::fromStdString("INSERT INTO patient VALUES ('%1','%2',%3,'%4','%5','%6')")
                                   .arg(jsonObj["ID"].toString()).arg(jsonObj["Name"].toString())
                        .arg(jsonObj["Age"].toInt()).arg(jsonObj["DoctorID"].toString())
                        .arg(jsonObj["PhotoDate"].toString())
                        .arg(jsonObj["ImageListURL"].toString()));
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

void MainWindow::createToolButton()
{
    ui->BrushtoolButton->setIconSize(QSize(50,40));
    ui->BrushtoolButton->setText("Brush");
    ui->BrushtoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->ColortoolButton->setIconSize(QSize(100,30));
    ui->ColortoolButton->setText("Color");
    ui->ColortoolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

/*테이블에 생성된 환자를 더블 클릭할 시 6번째(jsonObj["ImageListURL"].toString();) colum의 이미지 경로를 탐색*/
void MainWindow::on_patientTableView_doubleClicked(const QModelIndex &index)
{
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
            /*이미지를 다운로드 받을 수 있는 변수 재할당*/
            downLoader = new Downloader(this);
            /*이미지 URL, 다운로드 받을 폴더 명, 이미지 파일 이름*/
            downLoader->setFile(ImageURL, "./Images/", ImageName);

            /*업로드 완료 시그널후 메인 윈도우의 listWidget에서 이미지를 업로드 받는 슬롯*/
            connect(downLoader, &Downloader::sendUpload, this, &MainWindow::receiveupload);
            qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

        }

        reply->deleteLater();
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

