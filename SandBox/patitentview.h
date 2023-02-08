#ifndef PATITENTVIEW_H
#define PATITENTVIEW_H

#include <QObject>
#include <QEventLoop>

class Downloader;
class MainWindow;
class QNetworkAccessManager;

class PatitentView : public QObject
{
    Q_OBJECT
public:
    explicit PatitentView(QObject *parent = nullptr);
    void patientView(QString = "", QString = "", QString = "");
    ~PatitentView();
private:
    Downloader *downLoader;
    MainWindow *mainWindow;

    QNetworkAccessManager* manager;
    QEventLoop eventLoop;

signals:
    void middlePatient();
};

#endif // PATITENTVIEW_H
