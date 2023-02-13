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

/*UploadDlg로부터 받은 URL접근 후 form-data 형식으로 서버에 이미지를 업로드하기 위한 함수*/
void Uploader::uploadImage(QString FileName, QString URL, QString FieldName)
{
    QUrl serviceUrl = QUrl(URL);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QNetworkRequest request(serviceUrl);

    /*이미지 정보들을 서버로 저장하기 위한 처리 과정*/
    QHttpPart imageName;
    imageName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ImageName\""));      // ImageName Key
    imageName.setBody(imageNametxt.toUtf8());                                                                       // ImageName Value
    multiPart->append(imageName);                                       //Key와 Value를 모두 입력하면 append하여 multipart 리스트에 추가

    QHttpPart patientName;
    patientName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"PatientName\""));  // PatientName Key
    patientName.setBody(patientNametxt.toUtf8());                                                                   // PatientName Value
    multiPart->append(patientName);

    QHttpPart pixelLength;
    pixelLength.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"PixelLength\""));  // PixelLength Key
    pixelLength.setBody(pixelLengthtxt.toUtf8());                                                                   // PixelLength Value
    multiPart->append(pixelLength);

    QHttpPart imageKinds;
    imageKinds.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ImageKinds\""));    // ImageKinds Key
    imageKinds.setBody(imageKindstxt.toUtf8());                                                                     // ImageKinds Value
    multiPart->append(imageKinds);

    QHttpPart photoDate;
    photoDate.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"PhotoDate\""));      // PhotoDate Key
    photoDate.setBody(photoDatetxt.toUtf8());                                                                       // PhotoDate Value
    multiPart->append(photoDate);


    /*이미지 파일을 업로드 하기 위한 특별 처리 과정*/
    file = new QFile(FileName);
    file->open(QIODevice::ReadOnly);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        "form-data; name=\""+FieldName+"\"; filename=\"" + imageParttxt + "\"");                // Key
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");                                       // mime-Type
    imagePart.setBodyDevice(file);                                                                              // Value
    multiPart->append(imagePart);                                   //Key와 Value를 모두 입력하면 append하여 multipart 리스트에 추가

    QNetworkAccessManager *manager = new QNetworkAccessManager;
    currentUpload = manager->post(request, multiPart);

    /*클라이언트에 저장된 이미지를 업로드하기 위한 QNetworkReply 함수들을 커넥트*/
    connect(currentUpload, &QNetworkReply::uploadProgress, this, &Uploader::onUploadProgress);
    connect(currentUpload, &QNetworkReply::finished, this, &Uploader::onUploadFinished);
}

void Uploader::onUploadProgress(qint64 bytesRead, qint64 byteTotal)
{

}
/*upload 완료시 해당 파일을 닫기*/
void Uploader::onUploadFinished()
{
    if(file->isOpen()){
        file->close();
        file->deleteLater();
    }
}

/*UploadDlg의 각 데이터를 받기 위한 슬롯 함수*/
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

