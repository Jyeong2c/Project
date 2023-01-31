#include "downloader.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>

/*Downloader클래스의 복사 생성자 내부의 manager 변수 new 할당*/
Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
}

Downloader::~Downloader()
{
    manager->deleteLater();
}

/*MainWindow의 downloader클래스의 setFile함수 파라미터 변수들을 토대로 file저장*/
void Downloader::setFile(QString fileURL, QString folderName, QString fileName)
{
    /*서버로 부터 받은 이미지 파일을 다운로드 하기위한 디렉토리 생성*/
    QDir dir;
    /*해당 디렉토리가 존재하는 경우 그대로 유지 /
      디렉토리가 없는 경우 folderName 그대로 새 디렉토리를 생성*/
    if(dir.exists(folderName)){
        qDebug() << "Existis: " + folderName;
    }else{
        /*mkdir = make directory*/
        dir.mkdir(folderName);
        qDebug() << "Created: " + folderName;
    }

    /*서버로 부터 받을 이미지 경로와 내부 파일 데이터를 png형태로 포멧*/
    QString filePath = fileURL;
    QStringList filePathList = filePath.split('/');
    QString fileExt = filePathList.at(filePathList.count() - 1);
    fileExt = "png";
    QString saveFilePath;
    saveFilePath = QString(folderName + "/" + fileName + "." + fileExt );

    /*요청받은 URL의 파일을 응답*/
    QNetworkRequest request;
    request.setUrl(QUrl(fileURL));
    reply = manager->get(request);

    /*서버로부터 받은 데이터를 파일로 전환*/
    file = new QFile;
    file->setFileName(saveFilePath);

    /*요청에 따른 서버 내 이미지 파일 다운로드와 완료여부를 확인하는 connect함수*/
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
}

/*다운로드 진행과정을 프로그래스 형태로 출력*/
void Downloader::onDownloadProgress(qint64 bytesRead,qint64 bytesTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() +" - "+ QString::number(bytesTotal).toLatin1());
}

/*다운로드 완료여부 출력 후 메인윈도우 listWidget 업로드*/
void Downloader::onFinished(QNetworkReply * reply)
{
    /*파일을 먼저 닫은 다음에 emit신호를 보내야 전체 이미지 리스트들을 부를 수 있음*/
    /*close() -> relpy()*/
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }

    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    switch(reply->error())
    {
    /*네트워크 요청에 에러사항이 없는 경우*/
    case QNetworkReply::NoError:
    {
        qDebug("file is downloaded successfully.");

        emit sendUpload();
        qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    }break;
    default:{
        qDebug(reply->errorString().toLatin1());
    };
    }
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
}

/*다운받는 파일을 기다리다가 파일을 WriteOnly형식으로 모두 읽어오기*/
void Downloader::onReadyRead()
{
    qDebug() << "Ready";
    file->open(QIODevice::WriteOnly);
    file->write(reply->readAll());
}

/*요청이 모두 끝난 경우 파일을 닫고 소멸*/
void Downloader::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}
