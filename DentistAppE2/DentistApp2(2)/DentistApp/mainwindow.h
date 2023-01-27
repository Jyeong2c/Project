#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scene.h"
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

 Layout *gridLayout;
private:
    Ui::MainWindow *ui;



   // Layout *gridLayout;
    FMX *fmx;
    Brightness * bright;

    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;
    int cnt =0;



signals:
//    void requestBrightUp();
//    void requestBrightLow();
//    void requestInverstion();
//    void requestVerticalFlip();
//    void requestHorizontalFlip();
//    void requestMono();
//    void requestZoomIn();
//    void requestZoomOut();
//    void requestLeftRotate();
//    void requestRightRotate();
//    void requestPanning();
//    void requestImageClear();
//    void requestBack();
//    void requestDark();
//    void requestDark1();
//    void requestDark2();


private slots:

    void on_brushToolButton_clicked();
    void on_rectangleToolButton_clicked();
    void on_colorToolButton_clicked();
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_circleToolButton_clicked();
    void on_arrowToolButton_clicked();
};
#endif // MAINWINDOW_H
