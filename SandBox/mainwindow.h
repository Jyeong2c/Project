#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qeventloop.h"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QAbstractTableModel>

class QTableWidgetItem;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequire;
class QNetworkReply;
class Downloader;
class PatientModel;
class Uploader;
class Blending;
class AddImageServer;
class DeleteImage;
class PatitentView;
class MainWidget;
class LoginDlg;
class QFile;
class QStackedWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PatientTableLoad();
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

    QString hostName = "192.168.0.12";
    QString portNum = "40000";

    QList<QString> patientList;

signals:
    //void sendPatientName(QString _list);
};


#endif // MAINWINDOW_H
