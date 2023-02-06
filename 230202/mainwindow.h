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
class Menuitem;
class TeethForm;
class QGraphicsItemGroup;

class Scene;



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


protected:
    void changeEvent(QEvent *event) override;                       // 윈도우 창 변경될 때
    void resizeEvent(QResizeEvent *event) override;                 // 위젯 창 변경될 때
    void sharpen();
    void brightness(int intensity);
    void contrast(int alpha);
    void inversion();
    void verticalFlip();
    void horizontalFlip();
    void currentImage();
    QGraphicsPixmapItem* pixItem;




public slots:
    void selectItem(QListWidgetItem*);                              // 이미지 선택
    void previousScreen();                                          // 이전 화면 (layout클래스 화면으로 돌아감)
    void slot_doctorInfo(QString DoctorID);



private slots:
    void on_layoutClearPushButton_clicked();
    void on_zoomInToolButton_clicked();
    void on_zoomOutToolButton_clicked();
    void on_leftRotateToolButton_clicked();
    void on_rightRotateToolButton_clicked();
    void on_sourceSizePushButton_clicked();

    void layoutSizeChanged(QGraphicsView *grid);
    void DoubleWidget(QGraphicsView *grid);




    void on_implantToolButton_clicked();

    void on_brightnessSlider_valueChanged(int);
    void on_contrastSlider_valueChanged(int);
    void on_sharpenToolButton_clicked();
    void on_inversionToolButton_clicked();
    void on_verticalFlipToolButton_clicked();
    void on_horizontalFlipToolButton_clicked();

    void on_processingClear_clicked();

    void on_brightnessSlider_sliderReleased();

    void on_actionSave_triggered();

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

    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    QHash<QGraphicsPixmapItem*, QString> itemHash;

    QString sendDoctorID;

    /*Processing*/
    QPixmap pix_image;
    QImage m_image;
    QImage m_imageCopy;
    QPixmap outPixImage;
    QPixmap imageClear;

    QImage outImg;
    int intensity = 0;
    float inten = 0;
    int alpha = 0.0;
    int test = 0;
    int value = 0;

//    QGraphicsItemGroup * m_pixmapGroup;
    QList<QGraphicsPixmapItem> list;





signals:    
    void sig_doctorInfo(QString);
    void sig_zoomKeySend();

};
#endif // MAINWINDOW_H
