#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEventLoop>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QAbstractTableModel>

//Qt에서 제공하는 클래스
class QTableWidgetItem;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequire;
class QNetworkReply;

//추가해야되는 사용자 클래스
class PatientModel;
class Blending;
class AddImageServer;
class DeleteImage;
class PatitentView;

//추가하지 않아도 되는 사용자 클래스
class MainWidget;
class LoginDlg;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*환자 정보 URL 접근 후 get으로 환자 목록 얻기*/
    void PatientTableLoad();
    /**/
    void loadImages();
public slots:
    void slotDoctorInfo(QString);

private slots:
    void receiveUpload();
    void on_DoctorButton_clicked();
    void on_postButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_actiondialog_triggered();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_actionLoginPage_triggered();

private:
    Ui::MainWindow *ui;
    //클래스 합류
    PatientModel *patientModel;
    Blending *blendingDialog;
    AddImageServer *uploadDialog;
    DeleteImage *delImage;
    PatitentView *patView;

    //이건 나중에
    MainWidget *mainWidget;
    LoginDlg *loginDlg;

    //지역변수
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QEventLoop eventLoop;

    //접속할 URL IP와 PORT 번호
    QString hostName = "192.168.0.12";
    QString portNum = "40000";
};


#endif // MAINWINDOW_H
