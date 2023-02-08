#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

class MainWindow;

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QDialog *parent = nullptr);
    ~LoginForm();

    MainWindow *m_window;

    bool toggle = false;

private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginForm *ui;

    QString hostName = "192.168.0.12";
    QString portNum = "40000";

signals:
    void sigLoginSuccess(int);
    void sigIDInfo(QString);
};

#endif // LOGINFORM_H
