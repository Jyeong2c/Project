#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Layout;
class FMX;

class QSqlQuery;
class QSqlTableModel;
class Brightness;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_brightToolButton_clicked();
    void on_brightnessToolButton_clicked();
    void on_inversionToolButton_clicked();
    void on_verticalFlipToolButton_clicked();
    void on_horizontalFlipToolButton_clicked();
    void on_sharpenToolButton_clicked();
    void on_horizontalSlider_rangeChanged(int min, int max);
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_sliderMoved(int position);
    void on_zoomInToolButton_clicked();

    void on_zoomOutToolButton_clicked();

    void on_panningToolButton_clicked();

    void on_leftRotateToolButton_clicked();

    void on_rightRotateToolButton_clicked();

    void on_horizontalSlider_actionTriggered(int action);

    void on_imagePushButton_clicked();

    void on_sharpenToolButton_toggled(bool checked);


    void on_sharpenToolButton_pressed();

    void on_sharpenToolButton_released();
    void sharpenOne();
    void sharpenTwo();
    void sharpenThree();


private:
    Ui::MainWindow *ui;

    Layout *gridLayout;
    FMX *fmx;
    Brightness * bright;

    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;
    int cnt =0;



signals:
    void requestBrightUp();
    void requestBrightLow();
    void requestInverstion();
    void requestVerticalFlip();
    void requestHorizontalFlip();
    void requestMono();
    void requestZoomIn();
    void requestZoomOut();
    void requestLeftRotate();
    void requestRightRotate();
    void requestPanning();
    void requestImageClear();
    void requestBack();
    void requestDark();
    void requestDark1();
    void requestDark2();

};
#endif // MAINWINDOW_H
