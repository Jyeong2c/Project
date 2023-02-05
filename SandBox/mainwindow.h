#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

class QTableWidgetItem;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequire;
class Downloader;
class QFile;

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

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);
    void receiveUpload();

    void on_DoctorButton_clicked();
    void on_postButton_clicked();

private:
    Ui::MainWindow *ui;
    Downloader *downLoader;
};

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    ~Downloader();
    void setFile(QString fileURL);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;


private slots:
    void onDownloadProgress(qint64, qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();

signals:
    void sendUpload();
};

#endif // MAINWINDOW_H
