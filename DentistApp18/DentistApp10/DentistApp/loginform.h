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


private:
    Ui::LoginForm *ui;

signals:
//    void sig_login(QString, QString);
    void sig_loginSuccess(int);

    void sig_idInfo(QString);
};

#endif // LOGINFORM_H
