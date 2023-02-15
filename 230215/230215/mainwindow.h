#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QEventLoop>

class Layout;
class FMX;

class QListWidgetItem;
class QGraphicsView;


class QSqlQuery;
class QSqlTableModel;
class QGraphicsPixmapItem;
class QStackedWidget;
class Maxlayout;
class LoginForm;
class KeyFeaturesForm;
class Menuitem;
class TeethForm;

class QMenu;    // 은지



class BlendingDlg;      // 재현
class UploadDlg;
class PatientModel;
class PatientView;
class DeleteImage;

class QNetworkAccessManager;
class QNetworkReply;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/* YUNA */
    void loadImages();  // listWidget에 파일 로드하는 함수
    void styleColor();  // ui 색상 및 크기 조절

/* EUN JI */
    QMenu *shapemenu;

    QAction* circleAction;
    QAction* rectangleAction;
    QAction* triangleAction;
    QAction* lineAction;
    QAction* arrowAction;


public slots:
/* YUNA */
    void selectItem(QListWidgetItem*);  // listWidget에서 이미지를 선택할 때 호출되는 슬롯
    void slot_doubleSize(); // 레이아웃에 있는 view 1,2,3,4를 더블 클릭했을 때 실행되는 슬롯
    void previousScreen();  // viewQuit(x버튼)을 눌렀을 때 실행되는 슬롯
    void slot_doctorInfo(QString DoctorID); // 로그인 할 때 실행되는 슬롯


private slots:
/* YUNA */
    void on_actionLayout_Clear_triggered(); // 레이아웃에 있는 view1, 2, 3, 4에 있는 모든 Scene이 초기화 됨
    void on_zoomInToolButton_clicked(); // 확대 기능 선택 시 실행되는 슬롯
    void on_zoomOutToolButton_clicked();    // 축소 기능 선택 시 실행되는 슬롯
    void on_rightRotateToolButton_clicked();    // 90°회전 기능 선택 시 실행되는 슬롯
    void on_leftRotateToolButton_clicked(); // -90°회전 기능 선택 시 실행되는 슬롯
    void on_actionSource_Size_triggered();  // 원본 크기 기능 선택 시 실행되는 슬롯
    void on_implantPushButton_clicked();    // 임플란트 기능 선택 시 실행되는 슬롯
    void on_actionImplant_Clear_triggered();    // 임플란트 초기화 기능 선택 시 실행되는 슬롯
    void on_logoutPushButton_clicked(); // 로그아웃 할 때 실행되는 슬롯



/* JAE YEONG */
//---------------------------------------------
    void on_brightnessSlider_valueChanged(int);
    void on_contrastSlider_valueChanged(int);
    void on_sharpenToolButton_clicked();
    void on_inversionToolButton_clicked();
    void on_verticalFlipToolButton_clicked();
    void on_horizontalFlipToolButton_clicked();
    void on_actionImage_Processing_Clear_triggered();
    void on_contrastSlider_sliderReleased();
    void on_actionSave_triggered();

    void brightnessButton();
    void darknessButton();
    void contrastButton();
//---------------------------------------------


/* EUN JI*/
//---------------------------------------------
    void circleButton();
    void triangleButton();
    void rectangleButton();
    void arrowButton();
    void lineButton();
    void deleteButton();
    void on_cursorToolButton_clicked();
    void on_brushToolButton_clicked();
    void on_colorPushButton_clicked();
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_actionDraw_Clear_triggered();
    void on_memoToolButton_clicked();
    void on_rotateDoubleSpinBox_valueChanged(double arg1);
//---------------------------------------------


/* JAE HYEON */
//---------------------------------------------
    void receiveDownload();                                     // 다운로드 완료 시 시그널을 받는 슬롯 함수
    void on_rulerToolButton_clicked();                          // 길이 측정 버튼 함수
    void on_protractorToolButton_clicked();                     // 각도 측정 버튼 함수
    void on_blendingPushButton_clicked();                       // 블렌딩 다이얼로그 호출 함수
    void on_patientTableView_clicked(const QModelIndex &index); // 환자 테이블 클릭시 발생하는 이벤트 함수
    void on_listWidget_doubleClicked(const QModelIndex &index); // 리스트 위젯의 이미지를 더블 클릭시 삭제하는 함수(좌/우 클릭 모두 가능)
    void on_actionUpload_triggered();                           // Upload 다이얼로그 호출 트리거

    /*각도, 길이 측정 결과를 받아오는 함수*/
    void reLengthMeasure(double length);
    void reAngleMeasure(double angle);
//---------------------------------------------



    void on_listWidget_clicked(const QModelIndex &index);

private:
/* YUNA */
    Ui::MainWindow *ui;

    /* 멤버 변수 */
    Layout *customLayout;   // Layout
    Maxlayout *myMaxlayout; // Maxlayout
    KeyFeaturesForm *keyFeatures;   // KeyFeaturesForm
    TeethForm *teeth;   // TeethForm
    QStackedWidget *stackWidget;

    int cnt = 0;

    QHash<QGraphicsPixmapItem*, QString> itemHash;



    /* JAE YEONG */
//---------------------------------------------
    /* 환자 정보 DB */
    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    void sharpen();
    void brightness(int intensity);
    void contrast(int alpha);
    void inversion();
    void verticalFlip();
    void horizontalFlip();
    void saveOutImage();

    /*Processing*/
    QImage m_image;
    QImage m_image1;
    QImage m_image2;
    QImage m_image3;
    QImage m_image4;
    QImage m_imageCopy1;
    QImage m_imageCopy2;
    QImage m_imageCopy3;
    QImage m_imageCopy4;

    QPixmap outPixImage;
    QGraphicsPixmapItem* pixItem;

    QImage outImg;
    int intensity = 0;
    float inten = 0;
    int alpha = 0.0;
    int c_value = 0;
    int beta = 0;
    double buttonAlpha = 2;
    int m_sharpenCnt = 0;
    int m_contrastCnt = 0;
//---------------------------------------------


/* JAE HYEON */
//-----------------------------------------------
    BlendingDlg *blending;                  // Blending 다이얼로그 클래스 변수
    UploadDlg *uploading;                   // Uploading 다이얼로그 클래스 변수
    PatientModel *patientModel;             // 환자 테이블 모델 클래스
    PatientView *patView;                   // 테이블 환자 클릭시 서버로부터 이미지를 다운로드하는 중계 클래스
    DeleteImage *delImage;                  // 서버의 이미지 정보와 이미지 파일을 삭제하는 클래스

    void PatientTableLoad();

    QString hostName = "192.168.0.12";
    QString portNum = "40000";

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QEventLoop eventLoop;


signals:
/* YUNA */
    void sig_logout(int);   // 로그인 페이지로 돌아가는 시그널
    void sig_logClear();


/* JAE YEONG */
//----------------------------
    void sig_bright(int);
    void sig_dark(int);

/* JAE HY */
    void sendImageWHP(int _width, int _height, double _pixels); // 이미지의 Width, Height, Pixels 데이터를 전송
};
#endif // MAINWINDOW_H
