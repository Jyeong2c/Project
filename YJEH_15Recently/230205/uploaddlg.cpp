#include "uploaddlg.h"
#include "ui_uploaddlg.h"
#include "uploader.h"

#include <QFileDialog>

UploadDlg::UploadDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadDlg)
{
    ui->setupUi(this);

    ui->ImageFileLineEdit->setReadOnly(true);
    upLoader = new Uploader(this);
    /*Uploader 클래스에 보내는 데이터를 커넥트*/
    connect(this, &UploadDlg::sendImageName, upLoader, &Uploader::reImageName);
    connect(this, &UploadDlg::sendPatientName, upLoader, &Uploader::rePatientName);
    connect(this, &UploadDlg::sendPixelData, upLoader, &Uploader::rePixelData);
    connect(this, &UploadDlg::sendImageKind, upLoader, &Uploader::reImageKind);
    connect(this, &UploadDlg::sendPhotoDate, upLoader, &Uploader::rePhotoDate);
    connect(this, &UploadDlg::sendImageFile, upLoader, &Uploader::reImageFile);
}

UploadDlg::~UploadDlg()
{
    delete ui;
    delete upLoader;
}

/*파일 다이얼로그를 열게하는 함수*/
void UploadDlg::on_fileDialogButton_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,
                                               "choose file for add server",
                                               "D:/Panorama"); //다이얼로그 경로 변경
    ui->ImageFileLineEdit->setText(dir);
}

/*보내기 버튼 클릭 시 발생하는 함수*/
void UploadDlg::on_sendButton_clicked()
{
    /*라인 에디트에 있는 텍스트 정보들을 각 변수에 저장*/
    imageName = ui->imageNameLineEdit->text();
    patientName = ui->patientNameLineEdit->text();
    pixelLength = ui->pixelLengthLineEdit->text();
    imageKind = ui->imageKindsLineEdit->text();
    photoDate = ui->photoDateLineEdit->text();
    imageFile = ui->ImageFileLineEdit->text();


    /*해당 데이터가 공백인 경우 데이터를 보내지 않음(전송 무시)*/
    if(imageName.isEmpty() && patientName.isEmpty() && pixelLength.isEmpty()
            && imageKind.isEmpty() && photoDate.isEmpty() && imageFile.isEmpty()) {
        return;
    }
    else {
        /*Uploader class에 보내는 신호*/
        emit sendImageName(imageName);
        emit sendPatientName(patientName);
        emit sendPixelData(pixelLength);
        emit sendImageKind(imageKind);
        emit sendPhotoDate(photoDate);
        emit sendImageFile(imageFile);

        /*Uploder class의 uploadImage 클래스를 통해서 Url탐색*/
        upLoader->uploadImage(ui->ImageFileLineEdit->text(),
                              "http://" + hostName + ":" + portNum + "/api/image/store",
                              "ImageFile");

        /*전송완료 시 라인에디트 데이터 클리어*/
        ui->imageNameLineEdit->clear();
        ui->patientNameLineEdit->clear();
        ui->pixelLengthLineEdit->clear();
        ui->imageKindsLineEdit->clear();
        ui->photoDateLineEdit->clear();
        ui->ImageFileLineEdit->clear();
    }
    close(); // 보내기 완료 후 다이얼로그 창 닫음
}
