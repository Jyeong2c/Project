#ifndef UPLOADDLG_H
#define UPLOADDLG_H

#include <QDialog>

class Uploader;

namespace Ui {
class UploadDlg;
}

class UploadDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UploadDlg(QWidget *parent = nullptr);
    ~UploadDlg();

private:
    Ui::UploadDlg *ui;

    //업로드할 URL 함수내부에 들어갈 IP, Port
    QString hostName = "192.168.0.12";
    QString portNum = "40000";
    //업로드할 URL을 찾는 클래스
    Uploader *upLoader;

    //업로드할 이미지 정보들
    QString imageName;
    QString patientName;
    QString pixelLength;
    QString imageKind;
    QString photoDate;
    QString imageFile;


signals:
    //해당 데이터 정보를 업로더 클래스에 전송
    void sendImageName(QString);
    void sendPatientName(QString);
    void sendPixelData(QString);
    void sendImageKind(QString);
    void sendPhotoDate(QString);
    void sendImageFile(QString);


private slots:
    //form에서 만든 버튼 슬롯
    void on_sendButton_clicked();
    void on_fileDialogButton_clicked();
};

#endif // UPLOADDLG_H
