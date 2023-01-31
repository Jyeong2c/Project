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
class LoginForm;
class KeyFeaturesForm;

/*서버의 데이터를 다운로드 받기 위한 클래스 선정*/
class Downloader;
class QProgressDialog;
class QWebSocketServer;
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
    void loadImages();                  // 이미지 로드

    bool d = false;

    /*해당 디렉토리 내부의 파일과 디렉토리를 복사할 수 있는 함수*/
    void copyPath(QString, QString);
    /*해당 프로그램의 INI파일을 생성하기 위한 함수*/
    void writeSettings();
    void readSettings();

    /*길이측정시 처음으로 측정되는 x, y좌표값*/
    void setFirstX(int &x);
    void setFirstY(int &y);

protected:
    void changeEvent(QEvent *event) override;                       // 윈도우 창 변경될 때
    void resizeEvent(QResizeEvent *event) override;                 // 위젯 창 변경될 때



public slots:
    void selectItem(QListWidgetItem*);                              // 이미지 선택
    void previousScreen();                                          // 이전 화면 (layout클래스 화면으로 돌아감)
    void slot_doctorInfo(QString DoctorID);

    /*길이 측정 결과 시그널을 받는 슬롯*/
    void receiveLengthMeasure(double length);

    /*각도 측정 결과 시그널을 받는 슬롯*/
    void receiveAngleMeasure(double angle);


private slots:
    void on_layoutClearPushButton_clicked();
    void on_zoomInToolButton_clicked();
    void on_zoomOutToolButton_clicked();
    void on_leftRotateToolButton_clicked();
    void on_rightRotateToolButton_clicked();
    void on_imageSizeClearPushButton_clicked();

    void layoutSizeChanged(QGraphicsView *grid);
    void DoubleWidget(QGraphicsView *grid);
    void showContextMenu(const QPoint &);


    void on_patientTableView_doubleClicked(const QModelIndex &index);
    void on_blendingButton_clicked();
    void on_actionload_triggered();
    /*길이 측정 기능을 scene에 추가하는 함수*/
    void on_rulerToolButton_clicked();
    /*각도 측정 기능을 scene에 추가하는 함수*/
    void on_protractorToolButton_clicked();
    /*listWiget의 이미지를 클릭 시 해당하는 이미지의 정보를 나타내는 함수*/
    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *customLayout;
    FMX *fmx;
    Maxlayout *myMaxlayout;
    KeyFeaturesForm *keyFeatures;

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
    /*1픽셀당 실제 이미지의 길이 데이터를 보내는 함수*/
    void sendImagePixel(double pixel);


signals:
    void sig_doctorInfo(QString);

};



#endif // MAINWINDOW_H
