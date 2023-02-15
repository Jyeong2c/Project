#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>


class MainWindow;

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();


private slots:
    void on_loginToolButton_clicked();  // loginToolButton을 클릭했을 때 실행되는 슬롯
    void slot_logClear();   // logoutPushButton을 클릭했을 때 실행되는 슬롯


private:
    Ui::LoginForm *ui;


signals:
    void sig_loginSuccess(int); // mainWidow로 이동
    void sig_idInfo(QString);   // 로그인 ID를 시그널로 전달
};

#endif // LOGINFORM_H
