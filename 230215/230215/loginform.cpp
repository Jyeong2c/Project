#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>
#include <QStackedWidget>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    ui->verticalSpacer_3->changeSize(20, 70);
    ui->verticalSpacer_4->changeSize(20, 50);
    ui->verticalSpacer_5->changeSize(20, 10);
    ui->horizontalSpacer_3->changeSize(10, 18);

    QString image1(tr(":/trademark/D11.png"));
    ui->label->setPixmap(image1);

    ui->loginToolButton->setShortcut(Qt::Key_Return);   // 엔터를 눌렀을 때도 loginToolButton이 활성화되도록 설정
}

LoginForm::~LoginForm()
{
    delete ui;
}

/* loginToolButton을 클릭했을 때 실행되는 슬롯 */
void LoginForm::on_loginToolButton_clicked()
{
    QString DoctorID = ui->idLineEdit->text();  // 로그인 ID
    QString password = ui->passwordLineEdit->text();    // 로그인 비밀번호

    if(DoctorID == "osstem1" && password == "1234") {   // 입력한 ID와 비밀번호가 일치할 경우
        emit sig_loginSuccess(1);   // mainWidow로 이동
        emit sig_idInfo(DoctorID);  // 로그인 ID를 시그널로 전달
//        qobject_cast<QWidget*>(parent())->setGeometry(250, 70, 1350, 885);
        qobject_cast<QWidget*>(parent())->showMaximized();
    }

    else if(DoctorID == "osstem2" && password == "5678") {  // 입력한 ID와 비밀번호가 일치할 경우
        emit sig_loginSuccess(1);   // mainWidow로 이동
        emit sig_idInfo(DoctorID);  // 로그인 ID를 시그널로 전달
        qobject_cast<QWidget*>(parent())->setGeometry(250, 150, 1248, 681);
    }
    else    // 입력한 ID와 비밀번호가 일치하지 않는 경우
        QMessageBox::information(this, "Login", "다시 시도해주세요.");  // "다시 시도해주세요" 팝업창을 띄움
}

/* logoutPushButton을 클릭했을 때 실행되는 슬롯 */
void LoginForm::slot_logClear()
{
    ui->idLineEdit->clear();    // idLineEidt 초기화
    ui->passwordLineEdit->clear();  // passwordLineEdit 초기화
}

