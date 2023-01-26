#include "qdownloader.h"
#include "qobjectdefs.h"

QDownloader::QDownloader(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager;
}

QDownloader::~QDownloader()
{
    manager->deleteLater();
}

void QDownloader::setFile(QString fileURL)
{
    /*파일 경로 찾기 후 지정된 파일 경로에 이미지를 다운로드*/
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString fileName = filePathList.at(filePathList.count() - 1);
    saveFilePath = QString("./" + fileName);

    /*받아오는 이미지(파일)을 네트워크상에 요구*/
    QNetworkRequest request;
    request.setUrl(QUrl(fileURL));
    reply = manager->get(request);

    file = new QFile;
    file->setFileName(saveFilePath);
    file->open(QIODevice::WriteOnly);

    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void QDownloader::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() + " - " + QString::number(bytesTotal).toLatin1());
}
void QDownloader::onFinished(QNetworkReply* reply)
{
    switch(reply->error())
    {
    case QNetworkReply::NoError:
    {
        qDebug("file is download successfully.");
    }break;
    default:
    {
        qDebug(reply->errorString().toLatin1());
    }
    }
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}
void QDownloader::onReadyRead()
{
    file->write(reply->readAll());
}
void QDownloader::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}

