#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>
#include <QStackedWidget>

//재현
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    ui->horizontalSpacer->changeSize(300, 10);
    ui->leftHorizontalSpacer->changeSize(0, 20);
    ui->rightHorizontalSpacer->changeSize(300, 50);

    ui->idLineEdit->setFixedSize(150, 20);
    ui->passwordLineEdit->setFixedSize(150, 20);
    ui->loginToolButton->setFixedSize(50, 50);
    ui->label->resize(500, 700);

    ui->stackedWidget->setStyleSheet("background:rgb(32, 56, 100)");
    ui->idLineEdit->setStyleSheet("background:rgb(255, 255, 255)");
    ui->passwordLineEdit->setStyleSheet("background:rgb(255, 255, 255)");
    ui->loginToolButton->setStyleSheet("background:rgb(255, 255, 255)");

    QList<int> sizes;
    sizes << 1000 << 500;
    ui->splitter->setSizes(sizes);

    QString imagePath(tr(":/trademark/osstem.jpg"));
    ui->label->setPixmap(imagePath);

    ui->loginToolButton->setShortcut(Qt::Key_Return);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_loginToolButton_clicked()
{
#if 0
    emit sig_loginSuccess(1); // mainWidow로 이동
#else
//    QString DoctorID = ui->idLineEdit->text();
//    QString password = ui->passwordLineEdit->text();

//    if(DoctorID == "osstem1" && password == "1234") {
//        emit sig_loginSuccess(1); // mainWidow로 이동
//        emit sig_idInfo(DoctorID);

//        qDebug() << DoctorID;
//    }
//    else if(DoctorID == "osstem2" && password == "5678") {
//        emit sig_loginSuccess(1); // mainWidow로 이동
//        emit sig_idInfo(DoctorID);
//    }
//    else
//        QMessageBox::information(this, "Login", "다시 시도해주세요.");

    //재현
    QString DoctorID = ui->idLineEdit->text();
    QString password = ui->passwordLineEdit->text();


    QNetworkAccessManager mgr;
    QEventLoop eventLoop;
    QNetworkRequest req( QUrl( QString("http://" + hostName + ":" + portNum + "/api/login/") ) );
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

            QString findID = patientObj["_id"].toString();
            QString findDoctorID = patientObj["DoctorID"].toString();
            QString findDoctorName = patientObj["DoctorName"].toString();
            QString findPassword = patientObj["Password"].toString();

            //아이디와 비밀번호는 찾을 수 있는데 음...

            if(findDoctorID == DoctorID){
                if(findPassword == password){

                    emit sig_loginSuccess(1); // mainWidow로 이동
                    emit sig_idInfo(DoctorID);
                }
                else
                    QMessageBox::information(this, "Login", "다시 시도해주세요.");
            }
        }

    }

    delete reply;
#endif
}

void LoginForm::slot_logClear()
{
    ui->idLineEdit->clear();
    ui->passwordLineEdit->clear();
}

