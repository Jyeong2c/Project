#include "downloader.h"
#include "patientview.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

PatitentView::PatitentView(QObject *parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
}

PatitentView::~PatitentView()
{
    manager->deleteLater();
    downLoader->deleteLater();
}

void PatitentView::patientView(QString _hostName, QString _portNum, QString _patient)
{
    QNetworkRequest req( QUrl( QString("http://" + _hostName + ":" + _portNum + "/api/image/") ) );
    QNetworkReply *reply = manager->get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

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

            if(PatientName == _patient){
                qDebug() << ImageFile;

                QString csvString = ImageFile.section("\\", 1, 1);
                qDebug() << "csvString" << csvString; // output : *.png

                downLoader = new Downloader;
                connect(downLoader, &Downloader::sendUpload, this, &PatitentView::middlePatient);
                downLoader->setFile(QString("http://" + _hostName + ":" + _portNum + "/uploads/%1").arg(csvString));
            }
        }
        delete reply;
    }
}
