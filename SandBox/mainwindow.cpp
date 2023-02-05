#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFile>
#include <QDir>
#include <QTableWidgetItem>

//Json에 관련된 것
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include <QUrlQuery>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> list;
    list << 150 << 450;
    ui->splitter->setSizes(list);

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}


void MainWindow::PatientTableLoad()
{
    QList<QString> contacName;
    QList<int> age;
    QList<QString> doctorID;

    QNetworkAccessManager mgr;
    QEventLoop eventLoop;
    QNetworkRequest req( QUrl( QString("http://192.168.0.12:40000/api/patient/") ) );
    QNetworkReply *reply = mgr.get(req);

    connect(&mgr, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록

    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();

            QString ID = patientObj["_id"].toString();
            QString Name = patientObj["Name"].toString();
            int Age = patientObj["Age"].toInt();
            QString DoctorID = patientObj["DoctorID"].toString();

            qDebug() << Name;
            qDebug() << Age;
            qDebug() << DoctorID;

            if(ui->doctorIDLineEdit->text() == DoctorID){
                contacName.append(Name);
                age.append(Age);
                doctorID.append(DoctorID);
            }
        }

        delete reply;

        PatientModel *patientModel = new PatientModel(this);
        patientModel->populateData(contacName, age, doctorID);
        ui->tableView->setModel(patientModel);
        ui->tableView->horizontalHeader()->setVisible(true);

        ui->tableView->show();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QString path = "./Images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.png");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

    QString patient = index.data().toString();
    QNetworkAccessManager mgr;
    QEventLoop eventLoop;
    QNetworkRequest req( QUrl( QString("http://192.168.0.12:40000/api/image/") ) );
    QNetworkReply *reply = mgr.get(req);

    connect(&mgr, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록

    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));

        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();

            QString ID = patientObj["_id"].toString();
            QString ImageName = patientObj["ImageName"].toString();
            QString PatientName = patientObj["PatientName"].toString();
            double PixelLength = patientObj["PixelLength"].toDouble();
            QString ImageKinds = patientObj["ImageKinds"].toString();
            QString PhotoDate = patientObj["PhotoDate"].toString();
            QString ImageFile = patientObj["ImageFile"].toString();

            if(PatientName == patient){
                qDebug() << ImageFile;

                QString csvString = ImageFile.section("\\", 1, 1);
                qDebug() << "csvString" << csvString; // output : *.png

                downLoader = new Downloader;
                connect(downLoader, &Downloader::sendUpload, this, &MainWindow::receiveUpload);
                downLoader->setFile(QString("http://192.168.0.12:40000/uploads/%1").arg(csvString));
            }
        }
        delete reply;
    }
}


void MainWindow::loadImages()
{
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    /*해당 리스트 위젯을 클리어 한 후 환자의 이미지 리스트를 listWidget에 사진아이콘을 첨부하면서 출력*/
    ui->listWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon("./Images/" + fileInfoList.at(i).fileName())
                                                    , NULL, ui->listWidget);
        /*해당 아이콘 이미지를 클릭 시 트래킹으로 활성화하여 클릭시 해당 grid에 출력*/
        item->setStatusTip("./Images/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
}

void MainWindow::receiveUpload()
{
    loadImages();
}


PatientModel::PatientModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void PatientModel::populateData(const QList<QString> &_contactName,
                                const QList<int> &_age,
                                const QList<QString> &_doctorID)
{
    contactName.clear();
    contactName = _contactName;
    age.clear();
    age = _age;
    doctorID.clear();
    doctorID = _doctorID;
    return;
}

int PatientModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return contactName.length();
}
int PatientModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}
QVariant PatientModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole){
        return QVariant();
    }
    if(index.column() == 0){
        return contactName[index.row()];
    } else if (index.column() == 1) {
        return age[index.row()];
    } else if (index.column() == 2) {
        return doctorID[index.row()];
    }
    return QVariant();
}

QVariant PatientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        if(section == 0){
            return QString("Name");
        } else if(section == 1){
            return QString("age");
        } else if(section == 2){
            return QString("DoctorID");
        }
    }
    return QVariant();
}

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
}

Downloader::~Downloader()
{
    manager->deleteLater();
}

void Downloader::setFile(QString fileURL)
{
    /*서버로 부터 받은 이미지 파일을 다운로드 하기위한 디렉토리 생성*/
    QDir dir;
    /*해당 디렉토리가 존재하는 경우 그대로 유지 /
      디렉토리가 없는 경우 folderName 그대로 새 디렉토리를 생성*/
    if(dir.exists("./Images")){
        qDebug() << "Existis ./Images directory";
    }else{
        /*mkdir = make directory*/
        dir.mkdir("./Images");
        qDebug() << "Created ./Images directory";
    }

    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString fileName = filePathList.at(filePathList.count() - 1);
    saveFilePath = QString("./Images/" + fileName);

    QNetworkRequest request;
    request.setUrl(fileURL);
    reply = manager->get(request);

    file = new QFile;
    file->setFileName(saveFilePath);
    file->open(QIODevice::WriteOnly);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void Downloader::onDownloadProgress(qint64 bytesRead, qint64 byteTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() + " - " +
           QString::number(byteTotal).toLatin1());
}

void Downloader::onFinished(QNetworkReply* reply)
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
    switch(reply->error())
    {
    case QNetworkReply::NoError:
    {
        qDebug("file is downloaded successfully");
        emit sendUpload();
    }break;
    default:{
        qDebug(reply->errorString().toLatin1());
    };
    }
}

void Downloader::onReadyRead()
{
    file->write(reply->readAll());
}

void Downloader::onReplyFinished()
{
    if(file->isOpen()){
        file->close();
        file->deleteLater();
    }
}


void MainWindow::on_DoctorButton_clicked()
{
    //TableWidget 활성화
    PatientTableLoad();
}


void MainWindow::on_postButton_clicked()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager;
    const QString url = "192.168.0.12:40000/api/image/store";
    QUrlQuery postData;
    postData.addQueryItem("ImageName", "Paronama5");
    postData.addQueryItem("PatientName", "jeongdahyeon");
    postData.addQueryItem("PixelLength", QString::number(0.004));
    postData.addQueryItem("ImageKinds", "panorama");
    postData.addQueryItem("PhotoDate", "2023-02-06");

//    QFile file("D:/Panorama/panorama13.png");
//    QByteArray blob;
//    if(file.open(QIODevice::ReadOnly)){
//        blob = file.readAll();
//    }

    QFile f("D:/Panorama/panorama13.png");
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream stream(&f);
        QByteArray data;
        stream >> data;
        qDebug() << "blob size is" << data;

        postData.addQueryItem("ImageFile", data);
        postData.addQueryItem("data_type", "png");
    }
    //qDebug() << "blob size is" << blob.size();



    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "form-data");
    QNetworkReply *reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    reply->request();
}




