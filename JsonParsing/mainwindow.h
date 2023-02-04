#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qeventloop.h"
#include <QNetworkRequest>
#include <QMainWindow>

class QSettings;
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QEventLoop;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSettings *settings;
    QNetworkAccessManager *manager;  //웹페이지를 사용하기 위함


private slots:
    void on_AcessMakeButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkReply *rep;
    QNetworkRequest req;
    QEventLoop connection_loop;     //웹페이지를 호출하게 되면 이벤트로써 데이터 수신 완료 신호를 받기 때문에
                                    //동기화 프로그램을 위해 QEventLoop가 필요
};


#endif // MAINWINDOW_H
