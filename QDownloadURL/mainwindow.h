#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void checkUpdate();
    void doDownload(const QVariant& v);
    void downloadFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QList<QNetworkReply *> currentDownloads;

private slots:
    void updateDownloadProgress(qint64, qint64);
};
#endif // MAINWINDOW_H
