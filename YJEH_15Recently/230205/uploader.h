#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>

class QNetworkReply;
class QFile;
class AddImageServer;

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = 0);

    /*클라이언트의 이미지를 업로드하기 위한 uploadImage 함수*/
    void uploadImage(QString FileName, QString URL, QString FieldName);

public slots:
    /*UploadDlg의 각 데이터를 받기 위한 슬롯 함수*/
    void reImageName(QString);
    void rePatientName(QString);
    void rePixelData(QString);
    void reImageKind(QString);
    void rePhotoDate(QString);
    void reImageFile(QString);

private slots:
    /*서버의 이미지를 업로드 하기 위한 슬롯 함수*/
    void onUploadProgress(qint64, qint64);
    void onUploadFinished();

private:
    QNetworkReply* currentUpload;       // Upload전달 변수
    QFile* file;                        // 파일 처리 변수

    /*Upload 정보 변수*/
    QString imageNametxt;
    QString patientNametxt;
    QString pixelLengthtxt;
    QString imageKindstxt;
    QString photoDatetxt;
    QString imageParttxt;
};

#endif // UPLOADER_H
