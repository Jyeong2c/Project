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

    //웹페이지 수신 완료 신호를 받으면 QEventLoop의 quit() 메소드를 실행
    connect(manager, SIGNAL(finished(QNetworkReply *)),  &connection_loop, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_patientParse_clicked()
{
    QNetworkRequest req;
    req.setUrl(QUrl("http://192.168.0.12:4000/patient"));
    QNetworkReply *rep = manager->get(req);
    connection_loop.exec();

    QByteArray byteArray = rep->readAll();
    if(byteArray.isEmpty() == true) qDebug() << "Need to fill json Data";

    QJsonDocument docuMent = QJsonDocument::fromJson(byteArray);
    if(docuMent.isObject() == false) qDebug() << "It is not a Json object";

    QJsonArray patients = docuMent.array();

    QJsonObject jsonObject = docuMent.object();

    QString decodeData = "";

    for(int i = 0; i < patients.size(); i++)
    {
        QJsonArray patient = jsonObject["patient"].toArray();
        for(int i = 0; i < patient.size(); i++){
            QJsonObject jsonObj = patient.at(i).toObject();
            QJsonValue ID = jsonObj.value("ID");
            QJsonValue Name = jsonObj.value("Name");
            QJsonValue Age = jsonObj.value("Age");
            QJsonValue DoctorID = jsonObj.value("DoctorID");
            QJsonValue PhoneDate = jsonObj.value("PhoneDate");
            QJsonValue ImageListURL = jsonObj.value("ImageListURL");


            decodeData.append("Name : " + ID.toString() + "\n");
            decodeData.append("Surname : " + Name.toString() + "\n");
            decodeData.append("Age : " + QString::number(Age.toInt()) + "\n");
            decodeData.append("DoctorID : " + DoctorID.toString() + "\n");
            decodeData.append("PhoneDate : " + PhoneDate.toString() + "\n");
            decodeData.append("ImageListURL : " + ImageListURL.toString() + "\n");
        }
    }
    ui->plainTextEdit->setPlainText(decodeData);
}


void MainWindow::on_AcessMakeButton_clicked()
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    //네트워크 요구 변수
    QNetworkRequest req;
    req.setUrl(QUrl("http://127.0.0.1:3500/test/"));
    //네트워크 응답 변수
    QNetworkReply *rep = manager->get(req);

    connection_loop.exec();//동기화 부분
    QByteArray data = rep->readAll(); //해당 Url 전체 Json데이터 읽기
    if(data.isEmpty() == true) qDebug() << "Need to fill Json Data";

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isObject() == false) qDebug() << "It is not a Json object";

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



}




