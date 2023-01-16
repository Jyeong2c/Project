#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "scene.h"          //길이 측정 기능을 구현할 헤더 추가

class Layout;
class FMX;
class Listalbum;


class QSqlQuery;
class QSqlTableModel;

class QListWidget;
class QListWidgetItem;


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
    void loadImages();

public slots:
    void selectItem(QListWidgetItem*);
    void OrthoReceive(int x, int y);    //좌표 시그널을 받는 슬롯 함수

private slots:
    void on_patientTableView_doubleClicked(const QModelIndex &index);

    void on_RulertoolButton_triggered(QAction *arg1);

private:
    Ui::MainWindow *ui;

    /* 레이아웃 */
    Layout *customLayout;
    FMX *fmx;

    /* List Widget */
    Listalbum *listAlbum;

    /* 환자 정보 DB */
    void patientLoad();

    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;

    QListWidget *listWidget;

    //Scene 클래스
    Scene* scene;

    //선그리기와 선택 액션 지정
    QAction* lineAction;
    QAction* selectAction;

};
#endif // MAINWINDOW_H
