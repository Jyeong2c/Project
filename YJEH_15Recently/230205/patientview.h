#ifndef PATITENTVIEW_H
#define PATITENTVIEW_H

#include <QObject>
#include <QEventLoop>

/*환자 테이블 입력 시 이미지를 다운로드 할 클래스*/
class Downloader;
class QNetworkAccessManager;

class PatitentView : public QObject
{
    Q_OBJECT

public:
    explicit PatitentView(QObject *parent = nullptr);
    void patientView(QString = "", QString = "", QString = ""); // 해당 환자 이미지 다운로드
    ~PatitentView();

private:
    Downloader *downLoader;         // 이미지 다운로드 클래스 변수 할당

    QNetworkAccessManager* manager; // 다운로드 URL 경로
    QEventLoop eventLoop;

signals:
    void middlePatient();           // 이미지 다운로드 완료 중계 신호
};

#endif // PATITENTVIEW_H
