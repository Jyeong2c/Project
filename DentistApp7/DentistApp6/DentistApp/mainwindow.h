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
class Downloader;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class QDownloader;}
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


   ////////////////////////////////////////////////
private:
   Downloader* downLoader;

public slots:
   void receiveupload();
   void onFinished(QNetworkReply*);
};

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    virtual ~Downloader();
    void setFile(QString fileURL, QString folderName, QString fileName);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;

    //UI연동
    Ui::MainWindow *ui;

private slots:
    void onDownloadProgress(qint64,qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();

signals:
    void sendUpload();
};

#endif // MAINWINDOW_H
