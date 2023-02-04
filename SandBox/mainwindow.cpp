#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFile>
#include <QTableWidgetItem>

//Json에 관련된 것
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> list;
    list << 150 << 450;
    ui->splitter->setSizes(list);

    PatientTableLoad();

    downLoader = new Downloader(this);
}


void MainWindow::PatientTableLoad()
{
    ui->tableWidget->clear();
    QNetworkAccessManager mgr;
    QEventLoop eventLoop;
    QNetworkRequest req( QUrl( QString("http://192.168.0.12:40000/api/patient/") ) );
    QNetworkReply *reply = mgr.get(req);

    connect(&mgr, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록

    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();

            QString ID = patientObj["_id"].toString();
            QString Name = patientObj["Name"].toString();
            int Age = patientObj["Age"].toInt();
            QString DoctorID = patientObj["DoctorID"].toString();

            qDebug() << Name;
            qDebug() << Age;
            qDebug() << DoctorID;

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(Name));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(Age)));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(DoctorID));

        }

        delete reply;
        //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
        //테이블 Widget에 쓰는 Header resize code
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    QString patient = ui->tableWidget->currentIndex().data().toString();
    QNetworkAccessManager mgr;
    QEventLoop eventLoop;
    QNetworkRequest req( QUrl( QString("http://192.168.0.12:40000/api/image/") ) );
    QNetworkReply *reply = mgr.get(req);

    connect(&mgr, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록

    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();

            QString ID = patientObj["_id"].toString();
            QString ImageName = patientObj["ImageName"].toString();
            QString PatientName = patientObj["PatientName"].toString();
            double PixelLength = patientObj["PixelLength"].toDouble();
            QString ImageKinds = patientObj["ImageKinds"].toString();
            QString PhotoDate = patientObj["PhotoDate"].toString();
            QString ImageFile = patientObj["ImageFile"].toString();

            if(PatientName == patient){
                qDebug() << ImageFile;

                QString csvString = ImageFile.section("\\", 1, 1);
                qDebug() << "csvString" << csvString; // output *.png

                /*요청받은 URL의 파일을 응답*/
                downLoader->setFile(QString("http://192.168.0.12:40000/uploads/%1").arg(csvString));
            }
        }
        delete reply;
    }
}

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
}

Downloader::~Downloader()
{
    manager->deleteLater();
}

void Downloader::setFile(QString ImageFile)
{
    QNetworkRequest request;
    request.setUrl(QUrl(ImageFile));
    reply = manager->get(request);

    file = new QFile;
    //file->setFileName();
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
}

void Downloader::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() +" - "+ QString::number(bytesTotal).toLatin1());
}
void Downloader::onFinished(QNetworkReply*)
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

        //emit sendUpload();
        qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    }break;
    default:{
        qDebug(reply->errorString().toLatin1());
    };
    }
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
}
void Downloader::onReadyRead()
{
    qDebug() << "Ready";
    file->open(QIODevice::WriteOnly);
    file->write(reply->readAll());
}
void Downloader::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}


