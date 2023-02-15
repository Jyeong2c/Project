#ifndef DELETEIMAGE_H
#define DELETEIMAGE_H

#include <QObject>
#include <QEventLoop>

/*서버와 연결 하기 위한 Q클래스 선언*/
class QNetworkAccessManager;
class QNetworkReply;


class DeleteImage : public QObject
{
    Q_OBJECT
public:
    DeleteImage();
    ~DeleteImage();
    /*이미지 삭제함수 선언(hostName(IP), portNum, fileName) 초기값은 ""으로 설정*/
    void deleteImage(QString = "", QString = "", QString = "");


private:
    QNetworkAccessManager *manager;     // 서버 네트워크에 접근하기위한 manager 변수
    QEventLoop eventLoop;               // 서버의 동기화를 위한 변수
    QNetworkReply *reply;               // 서버의 url을 get으로 전달 받기 위한 변수

    QNetworkAccessManager *delImgInfoMgr; // 이미지정보를 삭제하기 위한 manager 변수
    QNetworkReply *delImgInfoRep;       // 이미지정보 삭제를 post로 전달 받기 위한 변수

    QNetworkAccessManager *delImgFileMgr; // 이미지파일을 삭제하기 위한 manager 변수
    QEventLoop eventImgLoop;             // 서버의 동기화를 위한 변수
    QNetworkReply *delImgFileRep;       // 이미지파일 삭제를 post로 전달 받기 위한 변수
};


#endif // DELETEIMAGE_H
