#ifndef QDOWNLOADER_H
#define QDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class QDownloader : public QObject
{
    Q_OBJECT
public:
    explicit QDownloader(QUrl imageUrl, QObject *parent = 0);
    virtual ~QDownloader();
    QByteArray downloadedData() const;

signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply* pReply);
private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
private slots:
    void loadImage();
};
#endif // QDOWNLOADER_H
