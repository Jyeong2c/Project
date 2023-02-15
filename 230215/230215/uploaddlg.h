#ifndef UPLOADDLG_H
#define UPLOADDLG_H

#include <QDialog>

class Uploader;         // 입력 받은 이미지 정보를 업로드 할 클래스

namespace Ui {
class UploadDlg;
}

/*클라이언트의 이미지 정보를 업로드 할 수 있도록 하는 다이얼로그 클래스*/
class UploadDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UploadDlg(QWidget *parent = nullptr);
    ~UploadDlg();

private slots:
    /*form에서 만든 버튼 슬롯*/
    void on_sendButton_clicked();
    void on_fileDialogButton_clicked();

signals:
    /*해당 데이터 정보를 업로더 클래스에 전송*/
    void sendImageName(QString);
    void sendPatientName(QString);
    void sendPixelData(QString);
    void sendImageKind(QString);
    void sendPhotoDate(QString);
    void sendImageFile(QString);

private:
    Ui::UploadDlg *ui;

    /*업로드할 URL 함수내부에 들어갈 IP, Port*/
    QString hostName = "192.168.0.12";
    QString portNum = "40000";

    Uploader *upLoader;   // 업로드할 URL을 찾는 클래스

    /*업로드할 이미지 정보들*/
    QString imageName;
    QString patientName;
    QString pixelLength;
    QString imageKind;
    QString photoDate;
    QString imageFile;

};

#endif // UPLOADDLG_H
