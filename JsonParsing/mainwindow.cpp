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
    req.setUrl(QUrl("http://192.168.0.12:4000/patientlist?by=osstem1"));

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
    QByteArray data = rep->readAll(); //해당 Url 전체 Json데이터 읽기
    if(data.isEmpty() == true) qDebug() << "Need to fill Json Data";

#if 1
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
        QJsonArray jsonArr  = doc1["array"].toArray();

        QString decodeData = "";
        for(int i = 0; i < jsonArr.size(); i++){
            QJsonObject jsonObj = jsonArr.at(i).toObject();
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
#else
    //JSON object와 data를 넣음
    QJsonObject object = doc.object();

    //Access the sibgle values : object안에 있는 array의 쌍수값을 접근
    QJsonValue name = object.value("name");
    QJsonValue surname = object.value("surname");
    QJsonValue age = object.value("age"); //이 값은 정수형 입니다.

    //Acess the inner Json Object and its inside data. object will be widthin {} curly braces
    //내부 Json 객체와 내부 데이터에 액세스합니다. 개체의 너비는 {}개의 중괄호로 표시됩니다
    QJsonObject address = object.value("address").toObject();
    QJsonValue addr_city = address.value("city");
    QJsonValue addr_country = address.value("country");

    //Acess the array within the JSON object. array will be with in [] squared braces
    //JSON 개체 내의 배열에 액세스합니다. 배열은 []개의 대괄호 내에 있습니다
    QJsonArray phones = object.value("phone").toArray();
    qDebug() << "There are " + QString::number(phones.size()) + " items in phones array";
    QString phoneList = "";
    for(unsigned int i = 0; i < phones.size(); i++)
        phoneList.append(phones.at(i).toString() + "\n");

    QString decodeData = "";

    decodeData.append("Name : " + name.toString() + "\n");
    decodeData.append("Surname : " + surname.toString() + "\n");
    decodeData.append("Age : " + QString::number(age.toInt()) + "\n");

    decodeData.append("Address : \n");
    decodeData.append("-City : " + addr_city.toString() + "\n");
    decodeData.append("-Country : " + addr_country.toString() + "\n");

    decodeData.append("Phones : \n" + phoneList + "\n");

    ui->plainTextEdit->setPlainText(decodeData);

    if(name.isString() == true) qDebug() << "name is a string";
    qDebug() << "Firstname : " + name.toString();
#endif

}




