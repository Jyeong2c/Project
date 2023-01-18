#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    manager = new QNetworkAccessManager;
}

MainWindow::~MainWindow()
{
}

void MainWindow::checkUpdate()
{
    QStringList files;
    files << "http://192.168.0.48:3000/Files/panorama4.png"
          << "http://192.168.0.48:3000/Files/panorama3.png";

    doDownload(files);
}

void MainWindow::doDownload(const QVariant& v)
{
    if(v.typeId() == QVariant::StringList){
        foreach(QString url, v.toStringList()){
            QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(url)));

            connect(manager, SIGNAL(downloadProgress(qint64, qint64)),
                    this, SLOT(updateDownloadProgress(qint64, qint64)));

            currentDownloads.append(reply);
        }
    }
}

void MainWindow::downloadFinished(QNetworkReply *reply)
{
    currentDownloads.removeAll(reply);
    reply->deleteLater();
}

void MainWindow::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    qDebug() << bytesRead << " - " << totalBytes;
}
