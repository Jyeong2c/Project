#include "addimageserver.h"
#include "ui_addimageserver.h"
#include "uploader.h"

#include <QFileDialog>

AddImageServer::AddImageServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddImageServer)
{
    ui->setupUi(this);
    ui->ImageFileLineEdit->setReadOnly(true);
    upLoader = new Uploader;
    //업로드 클래스에 보내는 데이터를 커넥트
    connect(this, &AddImageServer::sendImageName, upLoader, &Uploader::reImageName);
    connect(this, &AddImageServer::sendPatientName, upLoader, &Uploader::rePatientName);
    connect(this, &AddImageServer::sendPixelData, upLoader, &Uploader::rePixelData);
    connect(this, &AddImageServer::sendImageKind, upLoader, &Uploader::reImageKind);
    connect(this, &AddImageServer::sendPhotoDate, upLoader, &Uploader::rePhotoDate);
    connect(this, &AddImageServer::sendImageFile, upLoader, &Uploader::reImageFile);
}

AddImageServer::~AddImageServer()
{
    delete ui;
    delete upLoader;
}

/*파일 다이얼로그를 열게하는 함수*/
void AddImageServer::on_fileDialogButton_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,
                                               "choose file for add server",
                                               "D:/Panorama"); //다이얼로그 경로 변경
    ui->ImageFileLineEdit->setText(dir);
}

void AddImageServer::on_sendButton_clicked()
{
    imageName = ui->imageNameLineEdit->text();
    patientName = ui->patientNameLineEdit->text();
    pixelLength = ui->pixelLengthLineEdit->text();
    imageKind = ui->imageKindsLineEdit->text();
    photoDate = ui->photoDateLineEdit->text();
    imageFile = ui->ImageFileLineEdit->text();
    if(imageName.isEmpty() && patientName.isEmpty() && pixelLength.isEmpty()
            && imageKind.isEmpty() && photoDate.isEmpty() && imageFile.isEmpty()) {
        return;
    }
    else {
        emit sendImageName(imageName);
        emit sendPatientName(patientName);
        emit sendPixelData(pixelLength);
        emit sendImageKind(imageKind);
        emit sendPhotoDate(photoDate);
        emit sendImageFile(imageFile);

        upLoader->uploadImage(ui->ImageFileLineEdit->text(),
                              "http://" + hostName + ":" + portNum + "/api/image/store",
                              "ImageFile");
        ui->imageNameLineEdit->clear();
        ui->patientNameLineEdit->clear();
        ui->pixelLengthLineEdit->clear();
        ui->imageKindsLineEdit->clear();
        ui->photoDateLineEdit->clear();
        ui->ImageFileLineEdit->clear();
    }
}
