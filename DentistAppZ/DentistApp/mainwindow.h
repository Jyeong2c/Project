#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Layout;
class FMX;

class QSqlQuery;
class QSqlTableModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadImages();


private slots:
    void on_brightToolButton_clicked();
    void on_brightnessToolButton_clicked();
    void on_inversionToolButton_clicked();
    void on_patientTableView_doubleClicked(const QModelIndex &index);
    void on_protractorToolButton_clicked();
    void on_rulerToolButton_clicked();

private:
    Ui::MainWindow *ui;

    Layout *gridLayout;
    FMX *fmx;

    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

signals:
    void requestBrightUp();
    void requestBrightLow();
    void requestInverstion();
    void requestProtractor();
    void requestRuler();

};
#endif // MAINWINDOW_H
