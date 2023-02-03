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
    req.setUrl(QUrl("http://192.168.0.12:40000/api/patient/"));

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

#if 0
    //만일 Array부터 시작하는 경우
    QJsonParseError parseError;
    if(rep->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError){
            qDebug() << "Parse error: " << parseError.errorString();
            return;
        }
        if (!document.isArray()){
            qDebug() << "Document does not contain array";
            return;
        }

        QJsonArray array = document.array();

        QString decodeData;
        for(int i = 0; i < array.size(); i++){
            QJsonObject jsonObj = array.at(i).toObject();
            QJsonObject patientObj = jsonObj.constFind("patient")->toObject();
            QString ID = patientObj["ID"].toString();
            QString Name = patientObj["Name"].toString();
            int Age = patientObj["Age"].toInt();
            QString DoctorID = patientObj["DoctorID"].toString();
            QString PhotoDate = patientObj["PhotoDate"].toString();
            QString ImageListURL = patientObj["ImageListURL"].toString();

            decodeData.append("ID : " + ID  + "\n");
            decodeData.append("Name : " + Name + "\n");
            decodeData.append("Age : " + QString::number(Age) + "\n");
            decodeData.append("DoctorID : " + DoctorID + "\n");
            decodeData.append("PhotoDate : " + PhotoDate + "\n");
            decodeData.append("ImageListURL : " + ImageListURL + "\n\n");

        }
        ui->plainTextEdit->setPlainText(decodeData);
    }
#elif 0
    if(rep->error() == QNetworkReply::NoError){
        QJsonDocument doc1 = QJsonDocument::fromJson(data);
        if(doc1.isObject() == false) qDebug() << "It is not a Json object";
        QJsonArray jsonArr  = doc1["response"].toArray();

        QString decodeData = "";
        for(int i = 0; i < jsonArr.size(); i++){
            QJsonObject patientObj = jsonArr.at(i).toObject();
            //QJsonObject patientObj = jsonObj.constFind("patient")->toObject();
            QString ID = patientObj["_id"].toString();
            QString Name = patientObj["Name"].toString();
            int Age = patientObj["Age"].toInt();
            QString DoctorID = patientObj["DoctorID"].toString();
            QString PhotoDate = patientObj["PhotoDate"].toString();
            QString ImageListURL = patientObj["ImageListURL"].toString();

            decodeData.append("ID : " + ID  + "\n");
            decodeData.append("Name : " + Name + "\n");
            decodeData.append("Age : " + QString::number(Age) + "\n");
            decodeData.append("DoctorID : " + DoctorID + "\n");
            decodeData.append("PhotoDate : " + PhotoDate + "\n");
            decodeData.append("ImageListURL : " + ImageListURL + "\n\n");

        }
        ui->plainTextEdit->setPlainText(decodeData);
    }
#elif 0
    //만일 Array부터 시작하는 경우
    QJsonParseError parseError;
    if(rep->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError){
            qDebug() << "Parse error: " << parseError.errorString();
            return;
        }
        if (!document.isArray()){
            qDebug() << "Document does not contain array";
            return;
        }

        QString decodeData;
        QJsonArray array = document.array();
        for(int i = 0; i < array.size(); i++){
            QJsonObject jsonObj = array.at(i).toObject();
            QJsonObject patientObj = jsonObj.constFind("Image")->toObject();
            double pixelLength = patientObj["PixelLength"].toDouble();
            QString Name = patientObj["ImageName"].toString();
            decodeData.append("Name : " + Name + "\n");
            decodeData.append("pixelLength : " + QString::number(pixelLength) + "\n");
            qDebug() << pixelLength;
        }

        ui->plainTextEdit->setPlainText(decodeData);
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
            QString PhotoDate = patientObj["PhotoDate"].toString();
            QString ImageListURL = patientObj["ImageListUrl"].toString();

            decodeData.append("ID : " + ID  + "\n");
            decodeData.append("Name : " + Name + "\n");
            decodeData.append("Age : " + QString::number(Age) + "\n");
            decodeData.append("DoctorID : " + DoctorID + "\n");
            decodeData.append("PhotoDate : " + PhotoDate + "\n");
            decodeData.append("ImageListUrl : " + ImageListURL + "\n\n");
#endif
        }
        ui->plainTextEdit->setPlainText(decodeData);
        delete rep;

    }

#endif

}
