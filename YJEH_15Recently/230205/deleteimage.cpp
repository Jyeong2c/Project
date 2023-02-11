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
    /*이미지 정보를 서버에서 탐색하는 매니저 변수 할당*/
    manager = new QNetworkAccessManager(this);
    /*이미지 정보를 서버에서 삭제하는 아이디를 post로 처리하는 변수 할당*/
    delImgInfoMgr = new QNetworkAccessManager(this);
    /*이미지 파일을 서버에서 삭제하려는 이미지 이름을 get으로 처리하는 변수 할당*/
    delImgFileMgr = new QNetworkAccessManager(this);

    /*각 매니저 변수의 동기화를 위한 이벤트 루프 커넥트 함수(post 문은 connect 사용 X)*/
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    connect(delImgFileMgr, SIGNAL(finished(QNetworkReply *)), &eventImgLoop, SLOT(quit()));
}

DeleteImage::~DeleteImage()
{
    /*각 매니저 변수 할당후 소멸*/
    delete manager;
    delete delImgInfoMgr;
    delete delImgFileMgr;
}

/*이미지 삭제함수 선언(hostName(IP), portNum, fileName) 초기값은 ""으로 설정*/
void DeleteImage::deleteImage(QString _hostName, QString _portNum, QString _fileName)
{
    QNetworkRequest req( QUrl( QString("http://" + _hostName + ":" + _portNum + "/api/image/") ) );
    reply = manager->get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

    /*네트워크 응답에 이상이 없으면 json데이터 파싱*/
    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));    // joso 포멧

        QJsonArray jsonArr = jsonResponse["response"].toArray();    // response 배열내부 데이터 파싱
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    // jsonResponse.object();

            QString ID = patientObj["_id"].toString();              // _id 데이터 추출
            QString ImageFile = patientObj["ImageFile"].toString(); // ImageFile 데이터 추출

            QString csvString = ImageFile.section("\\", 1, 1);      // ImageFile 내부의 uploads\\*.png 데이터를 csv하여 *.png로 추출

            ////////////////////////////////////////////////////////////////////////  해당 이미지 정보와 파일을 삭제하기 위한 매니저 Accessing
            if(csvString == _fileName){                 // 파라미터로 받은 이미지 파일이름과 csv 정보가 같은 경우
                /*이미지 정보 삭제*/
                const QUrl url("http://" + _hostName + ":" + _portNum + "/api/image/delete");
                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QJsonObject obj;
                obj["ImageID"] = ID;    // 이미지 정보 중 ID를 할당 받아 삭제하기 위한 변수 설정

                QJsonDocument doc(obj); // 설정된 아이디 변수를 doc 변수에 초기화
                QByteArray data = doc.toJson();

                /*받은 데이터를 post하여 reply*/
                delImgInfoRep = delImgInfoMgr->post(request, data);
                connect(delImgInfoRep, &QNetworkReply::finished, [=]() { // 람다식
                    if(delImgInfoRep->error() == QNetworkReply::NoError) {
                        QByteArray ba=delImgInfoRep->readAll();
                        QString contents = QString::fromUtf8(ba);
                    }
                    else {
                        //QString err = reply->errorString();
                    }
                    delete delImgInfoRep;
                });

                /*이미지 파일 삭제*/
                QNetworkRequest delImgReq(QUrl(QString("http://" + _hostName + ":" + _portNum + "/api/image/removeSync?pngFile=%1")
                                               .arg(csvString)));       // csv된 파일이름을 서버에서 찾아 삭제
                delImgFileRep = delImgFileMgr->get(delImgReq);
                eventImgLoop.exec();

                if(delImgFileRep->error() == QNetworkReply::NoError){
                    qDebug() << "Image Delete complete";
                }
                delete delImgFileRep;
            }
            ///////////////////////////////////////////////////////////
            /*이미지 폴더 경로 찾은 후 이미지 제거*/
            QString path = "./Images";
            QDir dir(path);
            dir.remove(_fileName);
        }
        delete reply;
    }
}
