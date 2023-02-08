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
    manager = new QNetworkAccessManager(this);
    delImgInfoMgr = new QNetworkAccessManager(this);
    delImgFileMgr = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    connect(delImgFileMgr, SIGNAL(finished(QNetworkReply *)), &eventImgLoop, SLOT(quit()));
}

DeleteImage::~DeleteImage()
{
    delete manager;
    delete delImgInfoMgr;
    delete delImgFileMgr;
}

void DeleteImage::deleteImage(QString _hostName, QString _portNum, QString _fileName)
{
    QString path = "./Images";
    QDir dir(path);
    dir.remove(_fileName);
    QNetworkRequest req( QUrl( QString("http://" + _hostName + ":" + _portNum + "/api/image/") ) );
    reply = manager->get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

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
                delImgInfoMgr = new QNetworkAccessManager();
                const QUrl url("http://" + _hostName + ":" + _portNum + "/api/image/delete");
                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QJsonObject obj;
                obj["ImageID"] = ID;

                QJsonDocument doc(obj);
                QByteArray data = doc.toJson();

                delImgInfoRep = delImgInfoMgr->post(request, data);
                connect(delImgInfoRep, &QNetworkReply::finished, [=](){ //람다식
                    if(delImgInfoRep->error() == QNetworkReply::NoError){
                        QByteArray ba=delImgInfoRep->readAll();
                        QString contents = QString::fromUtf8(ba);
                        qDebug()<<contents;
                    }
                    else{
                        QString err = reply->errorString();
                        qDebug() << err;
                    }
                    delete delImgInfoRep;
                });

                delImgFileMgr = new QNetworkAccessManager();
                QNetworkRequest delImgReq(QUrl(QString("http://" + _hostName + ":" + _portNum + "/api/image/removeSync?pngFile=%1")
                                         .arg(csvString)));
                delImgFileRep = delImgFileMgr->get(delImgReq);
                eventImgLoop.exec();

                if(delImgFileRep->error() == QNetworkReply::NoError){
                    qDebug() << "Image Delete complete";
                }
                delete delImgFileRep;
            }
            ///////////////////////////////////////////////////////////
        }
        delete reply;
    }
}
