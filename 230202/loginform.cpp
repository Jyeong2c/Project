#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>
#include <QStackedWidget>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    ui->horizontalSpacer->changeSize(300, 10);
    ui->leftHorizontalSpacer->changeSize(0, 20);
    ui->rightHorizontalSpacer->changeSize(300, 50);

    ui->idLineEdit->setFixedSize(150, 20);
    ui->passwordLineEdit->setFixedSize(150, 20);
    ui->loginToolButton->setFixedSize(50, 50);
    ui->label->resize(500, 700);

    ui->stackedWidget->setStyleSheet("background:rgb(32, 56, 100)");
    ui->idLineEdit->setStyleSheet("background:rgb(255, 255, 255)");
    ui->passwordLineEdit->setStyleSheet("background:rgb(255, 255, 255)");
    ui->loginToolButton->setStyleSheet("background:rgb(255, 255, 255)");

    QList<int> sizes;
    sizes << 1000 << 500;
    ui->splitter->setSizes(sizes);

    QString imagePath(tr(":/trademark/osstem.jpg"));
    ui->label->setPixmap(imagePath);

    ui->loginToolButton->setShortcut(Qt::Key_Return);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_loginToolButton_clicked()
{
#if 1
    emit sig_loginSuccess(1); // mainWidow로 이동
#else
    QString DoctorID = ui->idLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if(DoctorID == "osstem1" && password == "1234") {
        emit sig_loginSuccess(1); // mainWidow로 이동
        emit sig_idInfo(DoctorID);

        qDebug() << DoctorID;
    }
    else if(DoctorID == "osstem2" && password == "5678") {
        emit sig_loginSuccess(1); // mainWidow로 이동
        emit sig_idInfo(DoctorID);
    }
    else
        QMessageBox::information(this, "Login", "다시 시도해주세요.");



#endif
}


