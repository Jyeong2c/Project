#ifndef DOWNLOADER_H
#define DOWNLOADER_H


#include <QObject>

/*서버와 파일 다루기 위한 Qclass*/
class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;
class QFile;


class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    ~Downloader();
    void setFile(QString fileURL);      // 서버로부터 받을 파일을 다운받기 위한 함수

private slots:
    void onDownloadProgress(qint64, qint64);  // 프로그래스 정보를 받는 함수
    void onFinished(QNetworkReply *);          // 이미지의 다운로드가 마침을 알리는 함수
    void onReplyFinished();          // reply의 역할이 종료시 알리는 함수

signals:
    void sendDownload();          // 다운로드가 완료됐음을 알리는 시그널

private:
    QNetworkAccessManager *manager;     // 이미지 다운로드 매니저
    QNetworkReply *reply;               // url 전달 변수
    //QNetworkRequest* request;
    QFile *file;                        // 파일 처리 변수
    QProgressDialog *progressDlg;       // 프로그래스 처리 변수
};
#endif // DOWNLOADER_H
