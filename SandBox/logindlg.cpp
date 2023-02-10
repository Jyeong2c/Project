#include "logindlg.h"
#include "ui_logindlg.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QMessageBox>

LoginDlg::LoginDlg(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_pushButton_clicked()
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

            //아이디와 비밀번호는 찾을 수 있는데 음...

            if(inputDoctorID == DoctorID){
                qDebug() << "ID ok";
                if(inputDoctorPW == Password){

                    emit sigLoginSuccess(1);
                    emit sigIDInfo(DoctorID);
                }
                else
                    qDebug() << "Not found pw";
            }
        }

    }

    delete reply;
}

