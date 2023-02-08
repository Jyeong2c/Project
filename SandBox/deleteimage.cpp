#include "deleteimage.h"
#include <QDir>
#include <QFileInfoList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QJsonDocument>


DeleteImage::DeleteImage()
{

}

void DeleteImage::deleteImage(QString _hostName, QString _portNum, QString _fileName)
{
    QNetworkAccessManager mgr;
    QEventLoop eventLoop;
    QNetworkRequest req( QUrl( QString("http://" + _hostName + ":" + _portNum + "/api/image/") ) );
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
            QString ImageFile = patientObj["ImageFile"].toString();

            QString csvString = ImageFile.section("\\", 1, 1);
            qDebug() << "csvString" << csvString; // output : *.png
            ////////////////////////////////////////////////////////////////////////
            if(csvString == _fileName){
                QNetworkAccessManager *deleteMgr = new QNetworkAccessManager();
                const QUrl url("http://" + _hostName + ":" + _portNum + "/api/image/delete");
                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QJsonObject obj;
                obj["ImageID"] = ID;

                QJsonDocument doc(obj);
                QByteArray data = doc.toJson();

                QNetworkReply *deleteReply = deleteMgr->post(request, data);
                connect(deleteReply, &QNetworkReply::finished, [=](){ //람다식
                    if(deleteReply->error() == QNetworkReply::NoError){
                        QByteArray ba=deleteReply->readAll();
                        QString contents = QString::fromUtf8(ba);
                        qDebug()<<contents;
                    }
                    else{
                        QString err = reply->errorString();
                        qDebug() << err;
                    }
                    deleteReply->deleteLater();
                });

                QNetworkAccessManager *delImgMgr = new QNetworkAccessManager();
                QEventLoop delImgLoop;
                QNetworkRequest delImgReq(QUrl(QString("http://" + _hostName + ":" + _portNum + "/api/image/removeSync?pngFile=%1")
                                         .arg(csvString)));
                QNetworkReply *delImgRep = delImgMgr->get(delImgReq);
                connect(delImgMgr, SIGNAL(finished(QNetworkReply *)),
                        &delImgLoop, SLOT(quit()));
                delImgLoop.exec();

                if(delImgRep->error() == QNetworkReply::NoError){
                    qDebug() << "Image Delete complete";
                }
                delete delImgRep;

                QString path = "./Images";
                QDir dir(path);
                dir.remove(_fileName);

            }
            ///////////////////////////////////////////////////////////
        }
        delete reply;
    }
}
