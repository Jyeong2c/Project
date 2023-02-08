#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

class MainWindow;

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QDialog *parent = nullptr);
    ~LoginDlg();

    MainWindow *m_window;

    bool toggle = false;

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginDlg *ui;

    QString hostName = "192.168.0.12";
    QString portNum = "40000";

signals:
    void sigLoginSuccess(int);
    void sigIDInfo(QString);;
};

#endif // LOGINDLG_H
