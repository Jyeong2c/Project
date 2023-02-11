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

    MainWindow *m_window;

    bool t = false;

private slots:
    void on_loginToolButton_clicked();
    void slot_logClear();

private:
    Ui::LoginForm *ui;

    //재현
    QString hostName = "192.168.0.12";
    QString portNum = "40000";

signals:
//    void sig_login(QString, QString);
    void sig_loginSuccess(int);

    void sig_idInfo(QString);

};

#endif // LOGINFORM_H
