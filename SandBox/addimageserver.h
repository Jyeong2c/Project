#ifndef ADDIMAGESERVER_H
#define ADDIMAGESERVER_H

#include <QDialog>

class Uploader;

namespace Ui {
class AddImageServer;
}

class AddImageServer : public QDialog
{
    Q_OBJECT

public:
    explicit AddImageServer(QWidget *parent = nullptr);
    ~AddImageServer();

private:
    Ui::AddImageServer *ui;
    QString hostName = "192.168.0.12";
    QString portNum = "40000";
    Uploader *upLoader;

    QString imageName;
    QString patientName;
    QString pixelLength;
    QString imageKind;
    QString photoDate;
    QString imageFile;

signals:
    void sendImageName(QString);
    void sendPatientName(QString);
    void sendPixelData(QString);
    void sendImageKind(QString);
    void sendPhotoDate(QString);
    void sendImageFile(QString);
private slots:
    void on_sendButton_clicked();
    void on_fileDialogButton_clicked();
};

#endif // ADDIMAGESERVER_H
