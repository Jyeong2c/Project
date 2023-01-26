#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

class Layout;
class FMX;

class QListWidgetItem;
class QGraphicsView;
class QGraphicsScene;

class QSqlQuery;
class QSqlTableModel;
class QGraphicsPixmapItem;
class QStackedWidget;
class Maxlayout;

/*서버의 데이터를 다운로드 받기 위한 클래스 선정*/
class QProgressDialog;
class QWebSocketServer;
class Downloader;
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QFile;

/*blening class 설정*/
class Blending;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void styleColor();                  // ui 색상 조절
    void customLayoutLocation();        // customLayout의 graphicsView 위치 좌표값
    void loadImages();                  // 이미지 로드

    bool d = false;
    bool r = false;
    bool y = false;

    /*해당 디렉토리 내부의 파일과 디렉토리를 복사할 수 있는 함수*/
    void copyPath(QString, QString);

protected:
    void changeEvent(QEvent *event) override;                       // 윈도우 창 변경될 때
    void resizeEvent(QResizeEvent *event) override;                 // 위젯 창 변경될 때


public slots:
    void selectItem(QListWidgetItem*);
    void previousScreen();              // 이전 화면 (layout클래스 화면으로 돌아감)
    void screenSizeChanged();


private slots:
    void on_imageClearPushButton_clicked();

    void layoutSizeChanged(QGraphicsView *grid);
    void DoubleWidget(QGraphicsView *grid);


    void on_patientTableView_doubleClicked(const QModelIndex &index);
    void on_blendingButton_clicked();

    void on_actionload_triggered();

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *customLayout;
    FMX *fmx;
    Maxlayout *myMaxlayout;

    /* 이중 창 */
    QStackedWidget *stackWidget;

    int cnt = 0;

    /* 환자 정보 DB */
    void patientLoad();
    QString patientID;

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    QHash<QGraphicsPixmapItem*, QString> itemHash;

    ////////////////////////////////////////////////
 private:
    /*할당 받는 IP와 PORT번호를 설정하는 QString 인자*/
    QString hostName = "192.168.0.12";
    QString portNum = "3000";
    Downloader* downLoader;

    /*서버로부터 이미지 파일을 받을 변수들을 설정*/
    QNetworkAccessManager *manager; //네트워크를 통해 요청/응답을 시행하는 메니저 변수
    QNetworkRequest *request;       //네트워크를 통해 데이터를 요청하는 변수

    /*블랜딩 다이얼로그를 호출하는 변수 추가*/
    Blending* blendDialog;

 public slots:
    /*Download클래스에서 보낸 이미지 다운로드 완효 신호를 받는 슬롯*/
    void receiveupload();
    /*요청이 끝남에 반응하는 함수*/
    void onFinished(QNetworkReply*);


signals:
//    void sig_currentImage(int);
//    void sig_grid1(QGraphicsPixmapItem* pixItem);
//    void sig_test(QString);
};

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

#endif // MAINWINDOW_H
