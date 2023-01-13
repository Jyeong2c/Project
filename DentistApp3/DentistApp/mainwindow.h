#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Layout;
class FMX;
class Listalbum;


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

    void createToolButton();            // 버튼 크기 조절

private slots:
    void on_selectButton_clicked();

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *gridLayout;
    FMX *fmx;

    /* List Widget */
    Listalbum *album;





    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;
};
#endif // MAINWINDOW_H
