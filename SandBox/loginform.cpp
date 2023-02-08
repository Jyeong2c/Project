#include "loginform.h"
#include "ui_loginform.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_loginButton_clicked()
{
    QString inputDoctorID = ui->doctorIDLineEdit->text();
    QString inputDoctorPW = ui->passwordLineEdit->text();

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

            QString ID = patientObj["_id"].toString();
            QString DoctorID = patientObj["DoctorID"].toString();
            QString DoctorName = patientObj["DoctorName"].toString();
            QString Password = patientObj["Password"].toString();

            qDebug() << DoctorID;
            qDebug() << DoctorName;
            qDebug() << Password;

            if(inputDoctorID == DoctorID){
                qDebug() << "ID ok";
                if(inputDoctorPW == Password) {
                    qDebug() << "Password ok";
                    emit sigLoginSuccess(1);
                    emit sigIDInfo(inputDoctorID);
                }
                else
                    QMessageBox::information(this, "Password", "비밀번호가 잘못되었습니다. \n다시 시도해주세요.");
            }
            else
                QMessageBox::information(this, "Login", "ID가 없습니다. \n다시 시도해주세요.");

        }
    }
    delete reply;
}

