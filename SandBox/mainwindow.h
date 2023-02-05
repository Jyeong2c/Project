#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QAbstractTableModel>

class QTableWidgetItem;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequire;
class Downloader;
class PatientModel;
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
    void receiveUpload();

    void on_DoctorButton_clicked();
    void on_postButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Downloader *downLoader;
};

class PatientModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PatientModel(QObject *parent = 0);
    void populateData(const QList<QString> &_contactName,
                      const QList<int> &age,
                      const QList<QString> &_doctorID);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

private:
    QList<QString> contactName;
    QList<int> age;
    QList<QString> doctorID;
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
