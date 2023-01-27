#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

class Layout;
class FMX;
class Processing;

class QListWidgetItem;
class QGraphicsView;
class QGraphicsScene;

class QSqlQuery;
class QSqlTableModel;
class QGraphicsPixmapItem;
class QStackedWidget;
class Maxlayout;


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


protected:

public slots:
    void selectItem(QListWidgetItem*);
    void previousScreen();              // 이전 화면 (layout클래스 화면으로 돌아감)


private slots:
    void on_imageClearPushButton_clicked();

    void layoutSizeChanged(QGraphicsView *grid);
    void DoubleWidget(QGraphicsView *grid);


    void on_brightToolButton_clicked();

    void on_brightnessToolButton_clicked();

    void on_sharpenToolButton_clicked();

    void on_contrastToolButton_clicked();

    void on_verticalFlipToolButton_clicked();

    void on_horizontalFlipToolButton_clicked();

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *customLayout;
    FMX *fmx;
    Maxlayout *myMaxlayout;
    Processing * processing;
    int intensity = 0;
    int inten=0;
    int cnt=0;
    float alpha = 0.0f;
    QGraphicsScene* scene;
    QImage * image;

    /* 이중 창 */
    QStackedWidget *stackWidget;


    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    QHash<QGraphicsPixmapItem*, QString> itemHash;


signals:
//    void sig_currentImage(int);
//    void sig_grid1(QGraphicsPixmapItem* pixItem);
//    void sig_test(QString);
};
#endif // MAINWINDOW_H
