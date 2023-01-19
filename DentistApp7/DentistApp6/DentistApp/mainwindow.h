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

class QProgressDialog;
class QWebSocketServer;
class QDownloader;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createToolButton();            // 버튼 크기 조절
    void loadImages();

public slots:
    void selectItem(QListWidgetItem*);

public slots:
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


    ///////////////////////////////////////////////

#if 0
public slots:
    void acceptConnection();            /*파일 서버*/
    void readClient();

private:
    /*웹 서버 변수 설정*/
    QWebSocketServer* webServer;
    /*프로그래스 다이얼로그 변수 설정*/
    QProgressDialog* progressDialog;
    /*데이터를 받는 변수*/
    qint64 byteReceived;
    /*데이터 전체 사이즈*/
    qint64 totalSize;
    /*파일 생성 변수*/
    QFile* file;
    /*바이트 데이터를 받기위한 변수*/
    QByteArray inBlock;
#else
   QDownloader* downloader;
#endif
};

class QDownloader : public QObject
{
    Q_OBJECT
public:
    explicit QDownloader(QObject *parent = 0);
    virtual ~QDownloader();
    void setFile(QString fileURL, QString folderName, QString fileName);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;

private slots:
    void onDownloadProgress(qint64,qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();
};

#endif // MAINWINDOW_H
