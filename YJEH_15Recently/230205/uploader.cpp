#include "uploader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>

Uploader::Uploader(QObject *parent) :
    QObject(parent)
{

}

void Uploader::uploadImage(QString FileName, QString URL, QString FieldName)
{
    QUrl serviceUrl = QUrl(URL);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QNetworkRequest request(serviceUrl);

    QHttpPart imageName;
    imageName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ImageName\""));
    imageName.setBody(imageNametxt.toUtf8());
    multiPart->append(imageName);

    QHttpPart patientName;
    patientName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"PatientName\""));
    patientName.setBody(patientNametxt.toUtf8());
    multiPart->append(patientName);

    QHttpPart pixelLength;
    pixelLength.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"PixelLength\""));
    pixelLength.setBody(pixelLengthtxt.toUtf8());
    multiPart->append(pixelLength);

    QHttpPart imageKinds;
    imageKinds.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ImageKinds\""));
    imageKinds.setBody(imageKindstxt.toUtf8());
    multiPart->append(imageKinds);

    QHttpPart photoDate;
    photoDate.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"PhotoDate\""));
    photoDate.setBody(photoDatetxt.toUtf8());
    multiPart->append(photoDate);

    file = new QFile(FileName);
    file->open(QIODevice::ReadOnly);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\""+FieldName+"\"; filename=\"" + imageParttxt + "\"");
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    QNetworkAccessManager *manager = new QNetworkAccessManager;
    currentUpload = manager->post(request, multiPart);

    connect(currentUpload, &QNetworkReply::uploadProgress, this, &Uploader::onUploadProgress);
    connect(currentUpload, &QNetworkReply::finished, this, &Uploader::onUploadFinished);
}

void Uploader::onUploadProgress(qint64 bytesRead, qint64 byteTotal)
{

}
void Uploader::onUploadFinished()
{
    if(file->isOpen()){
        file->close();
        //file->deleteLater();
    }
}

void Uploader::reImageName(QString _imageName)
{
    imageNametxt = _imageName;
}
void Uploader::rePatientName(QString _patientName)
{
    patientNametxt = _patientName;
}
void Uploader::rePixelData(QString _pixelDate)
{
    pixelLengthtxt = _pixelDate;
}
void Uploader::reImageKind(QString _imageKind)
{
    imageKindstxt = _imageKind;
}
void Uploader::rePhotoDate(QString _photoDate)
{
    photoDatetxt = _photoDate;
}
void Uploader::reImageFile(QString _imageFile)
{
    imageParttxt = _imageFile;
}

