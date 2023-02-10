#ifndef ADDIMAGESERVER_H
#define ADDIMAGESERVER_H

#include <QDialog>

class Uploader;

namespace Ui {
class AddImageServer;
}

/*서버에 들어갈 이미지 정보와 이미지 파일 업로드를 다이얼로그로 시각화*/
class AddImageServer : public QDialog
{
    Q_OBJECT

public:
    explicit AddImageServer(QWidget *parent = nullptr);
    ~AddImageServer();

private:
    Ui::AddImageServer *ui;
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

    //해당 데이터 정보를 업로더 클래스에 전송
signals:
    void sendImageName(QString);
    void sendPatientName(QString);
    void sendPixelData(QString);
    void sendImageKind(QString);
    void sendPhotoDate(QString);
    void sendImageFile(QString);

    //form에서 만든 버튼 슬롯
private slots:
    void on_sendButton_clicked();
    void on_fileDialogButton_clicked();
};

#endif // ADDIMAGESERVER_H
