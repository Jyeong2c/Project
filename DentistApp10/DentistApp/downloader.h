#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;
class QFile;

/*Qt에서 클래스를 정의 할 때 앞 클래스에 Q를 붙이지 말 것!*/
class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    virtual ~Downloader();
    /*서버의 URL, 저장받을 폴더, 파일의 이름을 파라미터로 받아 저장하는 함수*/
    void setFile(QString fileURL, QString folderName, QString fileName);

private:
    /*서버로부터 이미지 파일을 받을 변수들을 설정*/
    QNetworkAccessManager *manager; //네트워크를 통해 요청을 보낼수 있는 메니저 변수
    QNetworkReply *reply;           //요청에 반환된 개체(파일)들을 저장하는 변수
    QFile *file;                    //서버로부터 받는 데이터를 파일 형태로 받는 변수

private slots:
    void onDownloadProgress(qint64,qint64); //파일마다 다운로드 받는 상태를 표시하는 함수
    void onFinished(QNetworkReply*);        //클라이언트의 요청에 반응하는 함수
    void onReadyRead();                     //요청 받은 파일을 다운로드 하는데 기다리다가 읽는 함수
    void onReplyFinished();                 //요청이 끝났음을 표시하는 함수

signals:
    void sendUpload();                      //요청받은 파일의 다운로드를 마치면 보내는 시그널
};
#endif // DOWNLOADER_H
