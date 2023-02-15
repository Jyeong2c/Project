#include "downloader.h"
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtTest/QtTest>
#include <QTest>

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;        // manager new 객체 할당
    file = new QFile;

    //request = new QNetworkRequest;
}

Downloader::~Downloader()
{
    manager->deleteLater();                     // manager 소멸
    //delete request;
}

/*서버로부터 받을 파일을 다운받기 위한 함수*/
void Downloader::setFile(QString fileURL)
{
    QDir dir;        // 서버로 부터 받은 이미지 파일을 다운로드 하기위한 디렉토리 생성
    /*해당 디렉토리가 존재하는 경우 그대로 유지 /
      디렉토리가 없는 경우 folderName 그대로 새 디렉토리를 생성*/
    if(dir.exists("./Images")) {
    }
    else {                          // ./Images 경로가 없으면 ./Images 경로 생성
        /*mkdir = make directory*/
        dir.mkdir("./Images");
    }

    /*url로부터 받은 이미지 정보를 ./Images에 저장*/
    QString filePath = fileURL;                                     // fileUrl Path
    QString saveFilePath;                                           // 저장할 파일 경로 변수 선언
    QStringList filePathList = filePath.split('/');                 // url 내부의 '/'를 구분 지음
    QString fileName = filePathList.at(filePathList.count() - 1);   // 마지막 구분 자에 해당하는 이미지 파일만 추출하는 변수 할당
    saveFilePath = QString("./Images/" + fileName);                 // ./Images 디렉테리 내부에 파일 데이터를 저장

    QNetworkRequest request;
    request.setUrl(fileURL);                // fileURL을 request로 받음
    reply = manager->get(request);          // url을 get으로 전달

    //file = new QFile;
    file->setFileName(saveFilePath);        // 저장 파일 경로의 파일 이름 설정
    file->open(QIODevice::WriteOnly);       // 파일을 받기위한 형태로 쓰는 함수


    /*Qt 에서 제공하는 reply, manager 시그널을 downloader.cpp에 사용*/
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));                 // Qt의 downloadProgress signal 연결
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));                        // Qt의 NetworkReply signal 연결
    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));         // Qt의 readyRead signal 연결
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));      // Qt의 finished signal 연결
}

/*프로그래스 정보를 받는 함수*/
void Downloader::onDownloadProgress(qint64 bytesRead, qint64 byteTotal)
{
    /*처리되는 일은 없음*/
}

/*이미지의 다운로드가 마침을 알리는 함수*/
void Downloader::onFinished(QNetworkReply* reply)
{
    /*파일이 열린 상태 이면 파일을 닫고 새로운 이미지를 다운로드*/
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
    switch(reply->error())    // 에러 응답 스위치
    {
    case QNetworkReply::NoError:  // 이상없으면
    {
        emit sendDownload();    // download 시그널 전송
    }break;
    default:{
    };
    }
}

/*다운받는 이미지를 대기하다가 완료되면 받는 함수*/
void Downloader::onReadyRead()
{
//    QNetworkRequest request;
//    const QString address = "http://192.168.0.12:40000/api/image/";
//    request.setUrl(address);
//    reply = manager->get(request);      // url에 get으로 전달
//    //reply = manager->get()
    file->write(reply->readAll());      // 파일 읽기
}

/*reply의 역할이 종료시 알리는 함수*/
void Downloader::onReplyFinished()
{
    /*reply 종료시 파일도 close로 전환*/
    if(file->isOpen()){
        file->close();
        file->deleteLater();
    }
}
