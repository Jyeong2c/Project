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

#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

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

    manager = new QNetworkAccessManager;

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
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    update();
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
    update();
}

void MainWindow::setFile(QString fileURL){
    /*파일 경로 찾기 후 지정된 파일 경로에 이미지를 다운로드*/
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString fileName = filePathList.at(filePathList.count() - 1);
    saveFilePath = QString("./" + fileName);

    /*받아오는 이미지(파일)을 네트워크상에 요구*/
    QNetworkRequest request;
    request.setUrl(QUrl(fileURL));
    nrePly = manager->get(request);

    file = new QFile;
    file->setFileName(saveFilePath);
    file->open(QIODevice::WriteOnly);

    connect(nrePly, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
    connect(nrePly, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(nrePly, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}      // 파일 설정 함수

void MainWindow::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() + " - " + QString::number(bytesTotal).toLatin1());
}
void MainWindow::onFinished(QNetworkReply* _reply)
{
    switch(_reply->error())
    {
    case QNetworkReply::NoError:
    {
        qDebug("file is download successfully.");
    }break;
    default:
    {
        qDebug(_reply->errorString().toLatin1());
    }
    }
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}
void MainWindow::onReadyRead()
{
    file->write(nrePly->readAll());
}
void MainWindow::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}


void MainWindow::selectItem(QListWidgetItem *item){
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
//int num = 1;
void MainWindow::on_patientTableView_doubleClicked(const QModelIndex &index)
{
    update();
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    loadImages();
    QFile::remove(QString("*.png"));
    qDebug() << "selectDB Data Double clicked!";
    //qDebug() << "clicked index : " << ui->patientTableView->doubleClicked(index);

    int row =  ui->patientTableView->currentIndex().row();
    int column = ui->patientTableView->currentIndex().column();

    qDebug() << row << " " << column;
    /*이미지 리스트 명을 변수로 선언 table(row, 5)에 위치한 데이터가 이미지 리스트 URL*/
    QString ImageListURLName = patientQueryModel->data(patientQueryModel->index(row, 5)).toString();
    qDebug() << ImageListURLName;

    /*스택 위의 임시 이벤트 루프(event loop)*/
    QEventLoop eventLoop;

    /*"finished()"가 불려지면 이벤트 루프를 종료*/
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)),
                     &eventLoop, SLOT(quit()));
    /*URL 접속 여부 확인*/

    /*더블클릭된 이미지 리스트 URL안의 이미지 Json 데이터 호출*/
    QNetworkRequest req(QString("%1").arg(ImageListURLName));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록

    /*이미지 URL을 저장하기 위한 vector*/
    QStringList files;
    QString saveFilePath;

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
            //qDebug( ) << "ImageName:" << jsonObj["ImageName"].toString( );
            //qDebug( ) << "PixelLength:" << jsonObj["PixelLength"].toDouble( );
            //qDebug( ) << "ImageKinds:" << jsonObj["ImageKinds"].toString();
            /*이게 제일 중요 해당 이미지를 이 URL에서 호출하도록 해야함*/
            qDebug( ) << "ImagePathURL:" << jsonObj["ImagePathURL"].toString(); //이미지 URL 호출

            qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

            files << jsonObj["ImagePathURL"].toString();
            doDownload(files);

#endif
        }

        delete reply;
    }
}

void MainWindow::doDownload(const QVariant& v)
{
    if(v.type() == QVariant::StringList){
        foreach(QString url, v.toStringList()){
            QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
            connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
            qDebug() << "url:" << url;
            //QString saveFilePath = QString("./" + url);
            currentDownloads.append(reply);

            setFile(url);

            loadImages();
        }
    }
}

void MainWindow::downloadFinished(QNetworkReply *reply)
{
    currentDownloads.removeAll(reply);
    reply->deleteLater();
}

