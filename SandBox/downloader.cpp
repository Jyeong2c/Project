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

void Downloader::setFile(QString fileURL)
{
    /*서버로 부터 받은 이미지 파일을 다운로드 하기위한 디렉토리 생성*/
    QDir dir;
    /*해당 디렉토리가 존재하는 경우 그대로 유지 /
      디렉토리가 없는 경우 folderName 그대로 새 디렉토리를 생성*/
    if(dir.exists("./Images")){
        qDebug() << "Existis ./Images directory";
    }else{
        /*mkdir = make directory*/
        dir.mkdir("./Images");
        qDebug() << "Created ./Images directory";
    }

    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString fileName = filePathList.at(filePathList.count() - 1);
    saveFilePath = QString("./Images/" + fileName);

    QNetworkRequest request;
    request.setUrl(fileURL);
    reply = manager->get(request);

    file = new QFile(this);
    file->setFileName(saveFilePath);
    file->open(QIODevice::WriteOnly);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void Downloader::onDownloadProgress(qint64 bytesRead, qint64 byteTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() + " - " +
           QString::number(byteTotal).toLatin1());
}

void Downloader::onFinished(QNetworkReply* reply)
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
    switch(reply->error())
    {
        case QNetworkReply::NoError:
        {
            emit sendUpload();
        }break;

        default:{
            //qDebug(reply->errorString().toLatin1());
        };
    }
}

void Downloader::onReadyRead()
{
    file->write(reply->readAll());
}

void Downloader::onReplyFinished()
{
    if(file->isOpen()){
        file->close();
        file->deleteLater();
    }
}
