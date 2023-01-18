#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*URL의 이미지를 다운받을 수 있는 헤더*/
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QStringList>

#include <QMainWindow>

class Layout;
class FMX;
class Listalbum;


class QSqlQuery;
class QSqlTableModel;

class QListWidget;
class QListWidgetItem;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setFile(QString fileURL);      // 파일 설정 함수

    void createToolButton();            // 버튼 크기 조절
    void loadImages();

    void doDownload(const QVariant& v);
    void downloadFinished(QNetworkReply *reply);

public slots:
    void selectItem(QListWidgetItem*);

    /*다운로드 되는 과정을 보여주는 함수*/
    void onDownloadProgress(qint64, qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();

private slots:
    void on_patientTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *customLayout;
    FMX *fmx;

    /* List Widget */
    Listalbum *listAlbum;

    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    QListWidget *listWidget;

    QString hostName = "192.168.0.48";
    QString portNum = "3000";

    /*네트워크 연동 변수*/
    QNetworkAccessManager *manager;
    QList<QNetworkReply *> currentDownloads;
    QNetworkReply *nrePly;
    QFile *file;
};
#endif // MAINWINDOW_H
