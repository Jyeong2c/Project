#include "mainwindow.h"
#include "patientmodel.h"
#include "blending.h"
#include "addimageserver.h"
#include "deleteimage.h"
#include "ui_mainwindow.h"
#include "patitentview.h"
#include "logindlg.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QTableWidgetItem>

//Json에 Parsing과 관련된 헤더
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QStandardItemModel>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QMessageBox>
#include <QStackedWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> list;
    list << 150 << 450;
    //UI 폼 세팅
    ui->splitter->setSizes(list);
    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //manager 변수 new 할당
    manager = new QNetworkAccessManager(this);
    //upload 다이얼로그 변수 new 할당
    uploadDialog = new AddImageServer(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
}

//환자 테이블 불러오기 함수
void MainWindow::PatientTableLoad()
{
    //환자 데이터를 리스트화
    QList<QString> contacName;
    QList<int> age;
    QList<QString> doctorID;

    //환자 정보 Url 검색
    QNetworkRequest req( QUrl( QString("http://" + hostName + ":" + portNum + "/api/patient/") ) );
    reply = manager->get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

    //에러메시지가 없는 경우
    if (reply->error( ) == QNetworkReply::NoError) {
        //해당 Url에 저장된 Json데이터를 읽은 후 response배열 내부의 jsonData Parsing
        QString strReply = (QString)reply->readAll( );
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8( ));
        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();
            QString ID = patientObj["_id"].toString();
            QString Name = patientObj["Name"].toString();
            int Age = patientObj["Age"].toInt();
            QString DoctorID = patientObj["DoctorID"].toString();

            if(ui->doctorIDLineEdit->text() == DoctorID) {
                contacName.append(Name);
                age.append(Age);
                doctorID.append(DoctorID);
            }
        }
        delete reply;
        //환자모델 데이터를 얻은 후 PatientModel 클래스에 환자데이터 삽입
        patientModel = new PatientModel(this);
        patientModel->patientData(contacName, age, doctorID);
        ui->tableView->setModel(patientModel);
        ui->tableView->horizontalHeader()->setVisible(true);
        ui->tableView->show();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    manager->deleteLater();

    //MainWindow가 닫아질 때 이미지 폴더 내의 png 파일들을 제거
    QString path = "./Images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.png");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()){
        dir.remove(dirFile);
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    //해당환자의 테이블을 클릭시 이미지 폴더의 png 파일들을 제거
    QString path = "./Images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.png");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

    //환자의 이름을 받는 변수 할당
    QString patient = index.data().toString();
    patView = new PatitentView;
    connect(patView, &PatitentView::middlePatient, this, &MainWindow::receiveUpload);
    //환자의 테이블을 나열하기위한 함수 호출 후 환자의 이름을 입력받으면 해당이름에 맞는 이미지 리스트 다운로드
    patView->patientView(hostName, portNum, patient);
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
        QListWidgetItem* item = new QListWidgetItem(QIcon("./Images/" + fileInfoList.at(i).fileName()), NULL, ui->listWidget);
        /*해당 아이콘 이미지를 클릭 시 트래킹으로 활성화하여 클릭시 해당 grid에 출력*/
        item->setStatusTip("./Images/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
}

void MainWindow::receiveUpload()
{
    loadImages();
}

void MainWindow::on_DoctorButton_clicked()
{
    //TableWidget 활성화
    PatientTableLoad();
}

void MainWindow::on_postButton_clicked()
{
    /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
    QMessageBox::StandardButton buttonReply;

    /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
    buttonReply = QMessageBox::question(this, "question", "Are you Open Upload Dialog?",
                                        QMessageBox::Yes | QMessageBox::No);

    /*버튼을 누를 시 Yes인 경우*/
    if(buttonReply == QMessageBox::Yes) {
        /*blendDialog 호출*/
        uploadDialog = new AddImageServer(this);

        uploadDialog->exec();

        PatientTableLoad();

    }
    else {
        /*호출하지 않음*/
        return;
    }

}

void MainWindow::on_actiondialog_triggered()
{
    QDir dir("./Images/");
    /*./Image 디렉토리가 없을경우*/
    if(dir.isEmpty()){
        QMessageBox::information(this, "Blending Dialog",
                                 "Do Not any show listWidget");
        /*./Image 디렉토리의 내용이 없다고 알리며 예외처리*/
        return;
    }else{
        /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
        QMessageBox::StandardButton buttonReply;

        /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
        buttonReply = QMessageBox::question(this, "question", "Are you Open Blending Dialog?",
                                            QMessageBox::Yes | QMessageBox::No);

        /*버튼을 누를 시 Yes인 경우*/
        if(buttonReply == QMessageBox::Yes) {
            /*blendDialog 호출*/
            blendingDialog = new Blending(this);
            blendingDialog->exec();
        } else {
            /*호출하지 않음*/
            return;
        }
    }
}


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    /*파일정보 리스트에 png, jpg, bmp파일들만 리스트정보에 넣어둠*/
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    /*png 파일을 baseName으로 */
    QString fileName = fileInfoList.at(index.row()).fileName(); //*.png
    /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
    QMessageBox::StandardButton buttonReply;

    /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
    buttonReply = QMessageBox::question(this, "question", "Are you Delete this image?",
                                        QMessageBox::Yes | QMessageBox::No);

    /*버튼을 누를 시 Yes인 경우*/
    if(buttonReply == QMessageBox::Yes) {
        delImage = new DeleteImage;
        /*삭제 URL과 삭제할 파일의 이름을 파라미터로 데이터를 보냄*/
        delImage->deleteImage(hostName, portNum, fileName);
        loadImages();
    } else {
        /*호출하지 않음*/
        return;
    }
}

void MainWindow::slotDoctorInfo(QString DoctorID)
{
    ui->doctorIDLineEdit->setText(DoctorID);
    /*환자 정보를 부르기 전에 로그인 플래그를 걸어줌*/
    PatientTableLoad();                                  // 환자 정보 DB 함수
}

void MainWindow::on_actionLoginPage_triggered()
{
    loginDlg = new LoginDlg;
    loginDlg->exec();
}

