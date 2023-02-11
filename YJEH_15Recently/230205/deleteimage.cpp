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
    //이미지 정보를 서버에서 탐색하는 매니저 변수 할당
    manager = new QNetworkAccessManager(this);
    //이미지 정보를 서버에서 삭제하는 아이디를 post로 처리하는 변수 할당
    delImgInfoMgr = new QNetworkAccessManager(this);
    //이미지 파일을 서버에서 삭제하려는 이미지 이름을 get으로 처리하는 변수 할당
    delImgFileMgr = new QNetworkAccessManager(this);

    //각 매니저 변수의 동기화를 위한 이벤트 루프 커넥트 함수(post 문은 connect 사용 X)
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    connect(delImgFileMgr, SIGNAL(finished(QNetworkReply *)), &eventImgLoop, SLOT(quit()));
}

DeleteImage::~DeleteImage()
{
    //각 매니저 변수 할당후 소멸
    delete manager;
    delete delImgInfoMgr;
    delete delImgFileMgr;
}

void DeleteImage::deleteImage(QString _hostName, QString _portNum, QString _fileName)
{
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
                connect(delImgInfoRep, &QNetworkReply::finished, [=]() { //람다식
                    if(delImgInfoRep->error() == QNetworkReply::NoError) {
                        QByteArray ba=delImgInfoRep->readAll();
                        QString contents = QString::fromUtf8(ba);
                    }
                    else {
                        //QString err = reply->errorString();
                    }
                    delete delImgInfoRep;
                });

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
            //이미지 폴더 경로 찾은 후 이미지 제거
            QString path = "./Images";
            QDir dir(path);
            dir.remove(_fileName);
        }
        delete reply;
    }
}
