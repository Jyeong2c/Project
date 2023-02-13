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

    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));         // 네트워크 동기화
}

PatitentView::~PatitentView()
{
    manager->deleteLater();
    downLoader->deleteLater();
}

/*해당 환자 이미지 다운로드*/
void PatitentView::patientView(QString _hostName, QString _portNum, QString _patient)
{
    /*이미지 정보 URL 접근 후 일치하는 환자 이름별 이미지 리스트 출력*/
    QNetworkRequest req( QUrl( QString("http://" + _hostName + ":" + _portNum + "/api/image/") ) );
    QNetworkReply *reply = manager->get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        QJsonArray jsonArr = jsonResponse["response"].toArray();  // response Array 내부의 데이터 파싱
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    // jsonResponse.object();

            QString PatientName = patientObj["PatientName"].toString();     // 서버에 저장된 환자 이름 변수 할당
            QString ImageFile = patientObj["ImageFile"].toString();         // 서버에 저장된 이미지 파일 경로 변수 할당

            if(PatientName == _patient){                          // 선택된 환자의 이름을 찾은 경우

                QString csvString = ImageFile.section("\\", 1, 1);   // uploads\\*.png => *.png 로 csv

                downLoader = new Downloader;
                connect(downLoader, &Downloader::sendDownload, this, &PatitentView::middlePatient);     // 다운로드 신호 중계
                downLoader->setFile(QString("http://" + _hostName + ":" + _portNum + "/uploads/%1").arg(csvString));  // 해당 이미지 서버 URL 접근 후 다운로드
            }
        }
        delete reply;
    }
}
