#include "downloader.h"
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QProgressDialog>

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
}

Downloader::~Downloader()
{
    manager->deleteLater();
    file->deleteLater();
}

/*서버로부터 받을 파일을 다운받기 위한 함수*/
void Downloader::setFile(QString fileURL)
{
    QDir dir;        // 서버로 부터 받은 이미지 파일을 다운로드 하기위한 디렉토리 생성
    /*해당 디렉토리가 존재하는 경우 그대로 유지 /
      디렉토리가 없는 경우 folderName 그대로 새 디렉토리를 생성*/
    if(dir.exists("./Images")) {
    }
    else {
        /*mkdir = make directory*/
        dir.mkdir("./Images");
    }

    /*url로부터 받은 이미지 정보를 ./Images에 저장*/
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString fileName = filePathList.at(filePathList.count() - 1);
    saveFilePath = QString("./Images/" + fileName);

    QNetworkRequest request;
    request.setUrl(fileURL);
    reply = manager->get(request);          // url에 get으로 전달

    file = new QFile(this);
    file->setFileName(saveFilePath);
    file->open(QIODevice::WriteOnly);       // 파일을 받기위한 형태로 쓰는 함수

    /*Qt 에서 제공하는 reply, manager 시그널을 downloader.cpp에 사용*/
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
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
    file->write(reply->readAll());      // 파일 상태를 전부 읽고 대기(1개)
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
