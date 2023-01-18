#include "qdownloader.h"
#include "qobjectdefs.h"

QDownloader::QDownloader(QUrl imageUrl, QObject *parent)
    : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

QDownloader::~QDownloader()
{
    //manager->deleteLater();
}

void QDownloader::fileDownloaded(QNetworkReply* pReply)
{
    QUrl imageUrl("http: //qt.digia.com/Documents/1/QtLogo.png");
    m_pImgCtrl = new FileDownloader(imageUrl, this);

    connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));

    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray QDownloader::downloadedData() const
{
    return m_DownloadedData;
}


