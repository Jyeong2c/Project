#ifndef DELETEIMAGE_H
#define DELETEIMAGE_H

#include <QObject>
#include <QEventLoop>

class QNetworkAccessManager;
class QNetworkReply;

class DeleteImage : public QObject
{
    Q_OBJECT
public:
    DeleteImage();
    ~DeleteImage();
    void deleteImage(QString = "", QString = "", QString = "");
private:
    QNetworkAccessManager *manager;
    QEventLoop eventLoop;
    QNetworkReply *reply;

    QNetworkAccessManager *delImgInfoMgr;
    QNetworkReply *delImgInfoRep;

    QNetworkAccessManager *delImgFileMgr;
    QEventLoop eventImgLoop;
    QNetworkReply *delImgFileRep;
};

#endif // DELETEIMAGE_H
