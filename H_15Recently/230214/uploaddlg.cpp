#include "uploaddlg.h"
#include "ui_uploaddlg.h"
#include "uploader.h"

#include <QtTest/QtTest>
#include <QTest>
#include <QFileDialog>

UploadDlg::UploadDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadDlg)
{
    ui->setupUi(this);

    ui->ImageFileLineEdit->setReadOnly(true);   // upload할 파일 경로 라인 에디트는 readonly 상태
    upLoader = new Uploader(this);              // uploader 클래스 변수 할당

    /*라인 에디트에 있는 텍스트 정보들을 각 변수에 저장*/
    imageName = ui->imageNameLineEdit->text();          // imageName 변수를 택스트 형태로 선언
    patientName = ui->patientNameLineEdit->text();      // patientName 변수를 텍스트 형태로 선언
    pixelLength = ui->pixelLengthLineEdit->text();      // pixelLength 변수를 텍스트 형태로 선언
    imageKind = ui->imageKindsLineEdit->text();         // imageKind 변수를 텍스트 형태로 선언
    photoDate = ui->photoDateLineEdit->text();          // photoDate 변수를 텍스트 형태로 선언
    imageFile = ui->ImageFileLineEdit->text();          // imageFile 변수를 텍스트 형태로 선언

    /*Uploader 클래스에 보내는 데이터를 커넥트*/
    connect(this, &UploadDlg::sendImageName, upLoader, &Uploader::reImageName);     // 이미지 이름 정보를 Uploader 클래스로 전달
    connect(this, &UploadDlg::sendPatientName, upLoader, &Uploader::rePatientName); // 환자 성함 정보를 Uploader 클래스로 전달
    connect(this, &UploadDlg::sendPixelData, upLoader, &Uploader::rePixelData);     // 픽셀 당 길이 정보를 Uploader 클래스로 전달
    connect(this, &UploadDlg::sendImageKind, upLoader, &Uploader::reImageKind);     // 이미지 종류 정보를 Uploader 클래스로 전달
    connect(this, &UploadDlg::sendPhotoDate, upLoader, &Uploader::rePhotoDate);     // 이미지 날짜 정보를 Uploader 클래스로 전달
    connect(this, &UploadDlg::sendImageFile, upLoader, &Uploader::reImageFile);     // 이미지 파일 경로를 Uploader 클래스로 전달
}

UploadDlg::~UploadDlg()
{
    delete ui;
    //delete upLoader;
}

/*파일 다이얼로그를 열게하는 함수*/
void UploadDlg::on_fileDialogButton_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,
                                               "choose file for add server",
                                               "D:/Panorama"); //다이얼로그 경로 변경
    ui->ImageFileLineEdit->setText(dir);       // 파일 다이얼로그 버튼으로 서버에 보낼 이미지 경로 표시
}

/*보내기 버튼 클릭 시 발생하는 함수*/
void UploadDlg::on_sendButton_clicked()
{


    /*해당 데이터가 공백인 경우 데이터를 보내지 않음(전송 무시)*/
    if(imageName.isEmpty() && patientName.isEmpty() && pixelLength.isEmpty()
            && imageKind.isEmpty() && photoDate.isEmpty() && imageFile.isEmpty()) {
        return;
    }
    else {
        /*Uploader class에 보내는 신호*/
        emit sendImageName(imageName);          // 이미지 이름을 Uploader 클래스에 전송
        emit sendPatientName(patientName);      // 환자 성함을 Uploader 클래스에 전송
        emit sendPixelData(pixelLength);        // 픽셀당 길이 정보를 Uploader 클래스에 전송
        emit sendImageKind(imageKind);          // 이미지 종류 정보를 Uploader 클래스에 전송
        emit sendPhotoDate(photoDate);          // 이미지 촬영 날짜를 Uploader 클래스에 전송
        emit sendImageFile(imageFile);          // 이미지 파일

        /*Uploder class의 uploadImage 클래스를 통해서 Url탐색*/
        upLoader->uploadImage(ui->ImageFileLineEdit->text(),
                              "http://" + hostName + ":" + portNum + "/api/image/store",
                              "ImageFile");

        /*전송완료 시 라인에디트 데이터 클리어*/
        ui->imageNameLineEdit->clear();         // 전송 완료시 이미지 이름 라인 에디트 클리어
        ui->patientNameLineEdit->clear();       // 전송 완료시 환자 성함 라인 에디트 클리어
        ui->pixelLengthLineEdit->clear();       // 전송 완료시 픽셀 당 길이 정보 라인 에디트 클리어
        ui->imageKindsLineEdit->clear();        // 전송 완료시 이미지 종류 정보 라인 에디트 클리어
        ui->photoDateLineEdit->clear();         // 전송 완료시 사진 날짜 정보 라인 에디트 클리어
        ui->ImageFileLineEdit->clear();         // 전송 완료시 이미지 파일 경로 라인 에디트 클리어
    }
    close(); // 보내기 완료 후 다이얼로그 창 닫음
}
