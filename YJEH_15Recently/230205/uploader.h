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

    //image upload
    void uploadImage(QString FileName, QString URL, QString FieldName);

public slots:
    void reImageName(QString);
    void rePatientName(QString);
    void rePixelData(QString);
    void reImageKind(QString);
    void rePhotoDate(QString);
    void reImageFile(QString);

private slots:
    void onUploadProgress(qint64, qint64);
    void onUploadFinished();

private:
    QNetworkReply* currentUpload;
    QFile* file;

    QString imageNametxt;
    QString patientNametxt;
    QString pixelLengthtxt;
    QString imageKindstxt;
    QString photoDatetxt;
    QString imageParttxt;
};

#endif // UPLOADER_H
