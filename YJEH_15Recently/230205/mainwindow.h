#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
//재현
///////////////////
#include <QEventLoop>
#include <QAbstractTableModel>

//Qt에서 제공하는 클래스
class QTableWidgetItem;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequire;
class QNetworkReply;

//추가해야되는 사용자 클래스
class PatientModel;
class PatitentView;
class UploadDlg;
class DeleteImage;
///////////////////

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
class BlendingDlg;



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
    void PatientTableLoad();            // 재현

    bool d = false;


public slots:
    void selectItem(QListWidgetItem*);                              // 이미지 선택
    void previousScreen();                                          // 이전 화면 (layout클래스 화면으로 돌아감)
    void slot_doctorInfo(QString DoctorID);

    void slot_doubleSize();            // 최대창 화면에 맞게 scene의 크기를 조절해주는 슬롯





private slots:
    void on_layoutClearPushButton_clicked();
    void on_zoomInToolButton_clicked();
    void on_zoomOutToolButton_clicked();
    void on_leftRotateToolButton_clicked();
    void on_rightRotateToolButton_clicked();
    void on_sourceSizePushButton_clicked();
    void on_implantToolButton_clicked();
    void on_implantClearToolButton_clicked();
    void on_logoutPushButton_clicked();



/* JAE YEONG */
//---------------------------------------------
    void on_brightnessSlider_valueChanged(int);
    void on_contrastSlider_valueChanged(int);
    void on_sharpenToolButton_clicked();
    void on_inversionToolButton_clicked();
    void on_verticalFlipToolButton_clicked();
    void on_horizontalFlipToolButton_clicked();
    void on_processingClearPushButton_clicked();
    void on_contrastSlider_sliderReleased();
    void on_saveToolButton_clicked();

    void brightnessButton();
    void darknessButton();
    void contrastButton();
//---------------------------------------------


/* EUN JI*/
//---------------------------------------------
    void on_cursorToolButton_clicked();
    void on_brushToolButton_clicked();
    void on_circleToolButton_clicked();
    void on_triangleToolButton_clicked();
    void on_rectangleToolButton_clicked();
    void on_arrowToolButton_clicked();
    void on_colorToolButton_clicked();
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_drawClearPushButton_clicked();
    void on_lineToolButton_clicked();
    void on_deletePushButton_clicked();
    void on_printPushButton_clicked();
    void on_rotateDoubleSpinBox_valueChanged(double arg1);
//---------------------------------------------


/* JAE HYEON */
//---------------------------------------------------------------------
    void receiveUpload();
    void on_rulerToolButton_clicked();
    void on_protractorToolButton_clicked();
    void on_blendingPushButton_clicked();
    void on_patientTableView_clicked(const QModelIndex &index);
    void on_uploadButton_clicked();
    void on_listWidget_doubleClicked(const QModelIndex &index);

    void reLengthMeasure(double length);
    void reAngleMeasure(double angle);
//---------------------------------------------------------------------

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *customLayout;
    FMX *fmx;
    Maxlayout *myMaxlayout;
    KeyFeaturesForm *keyFeatures;
//    Menuitem *menuItem;

    /* 이중 창 */
    QStackedWidget *stackWidget;

    TeethForm *teeth;

    int cnt = 0;

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    QHash<QGraphicsPixmapItem*, QString> itemHash;


    QString sendDoctorID;


    /* JAE YEONG */
//---------------------------------------------
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

    /* JAE HY */
    //클래스 합류
    PatientModel *patientModel;
    PatitentView *patView;
    UploadDlg *uploading;
    BlendingDlg *blending;
    DeleteImage *delImage;
    //멤버변수
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QEventLoop eventLoop;

    //접속할 URL IP와 PORT 번호
    QString hostName = "192.168.0.12";
    QString portNum = "40000";


signals:    
    void sig_doctorInfo(QString);
    void sig_zoomKeySend();

    void sig_logout(int);
    void sig_logClear();

    void sig_bright(int);
    void sig_dark(int);
};
#endif // MAINWINDOW_H
