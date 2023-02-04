#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkRequest>
#include <QDebug>
#include <QEventLoop>
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

    manager = new QNetworkAccessManager(this);
    //네트워크 요구 변수
    //req.setUrl(QUrl("http://192.168.0.12:4000/allImagelist?by=panorama1"));
#if 1
    req.setUrl(QUrl("http://192.168.0.12:40000/api/image/"));
#else
    req.setUrl(QUrl("http://192.168.0.12:40000/api/patient/"));
#endif

    //웹페이지 수신 완료 신호를 받으면 QEventLoop의 quit() 메소드를 실행
    connect(manager, SIGNAL(finished(QNetworkReply *)), &connection_loop, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_AcessMakeButton_clicked()
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

    //네트워크 응답 변수
    rep = manager->get(req);

    connection_loop.exec();//동기화 부분

#if 1
    if (rep->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)rep->readAll( );
        // Json 파싱
        qDebug( ) << "Response:" << strReply;

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        //QJsonArray jsonArr  = doc1["response"].toArray();

        QString decodeData;
        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();
#if 0
            foreach(const QString& key, jsonObj.keys()) {
                QJsonValue value = jsonObj.value(key);
                qDebug() << "Key = " << key << ", Value = " << value.toString();
            }
#else
            QString ID = patientObj["_id"].toString();
            QString ImageName = patientObj["ImageName"].toString();
            QString PatientName = patientObj["PatientName"].toString();
            double PixelLength = patientObj["PixelLength"].toDouble();
            QString ImageKinds = patientObj["ImageKinds"].toString();
            QString PhotoDate = patientObj["PhotoDate"].toString();
            QString ImageFile = patientObj["ImageFile"].toString();

            decodeData.append("ID : " + ID  + "\n");
            decodeData.append("ImageName : " + ImageName + "\n");
            decodeData.append("PatientName : " + PatientName + "\n");
            decodeData.append("Age : " + QString::number(PixelLength) + "\n");
            decodeData.append("DoctorID : " + ImageKinds + "\n");
            decodeData.append("PhotoDate : " + PhotoDate + "\n");
            decodeData.append("ImageFile : " + ImageFile + "\n");
#endif
        }
        ui->plainTextEdit->setPlainText(decodeData);
        delete rep;

    }

#else
    if (rep->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)rep->readAll( );
        // Json 파싱
        qDebug( ) << "Response:" << strReply;

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        //QJsonArray jsonArr  = doc1["response"].toArray();

        QString decodeData;
        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();
#if 0
            foreach(const QString& key, jsonObj.keys()) {
                QJsonValue value = jsonObj.value(key);
                qDebug() << "Key = " << key << ", Value = " << value.toString();
            }
#else
            QString ID = patientObj["_id"].toString();
            QString Name = patientObj["Name"].toString();
            int Age = patientObj["Age"].toInt();
            QString DoctorID = patientObj["DoctorID"].toString();

            decodeData.append("ID : " + ID  + "\n");
            decodeData.append("Name : " + Name + "\n");
            decodeData.append("Age : " + QString::number(Age) + "\n");
            decodeData.append("DoctorID : " + DoctorID + "\n\n");
#endif
        }
        ui->plainTextEdit->setPlainText(decodeData);
        delete rep;

    }

#endif

}
