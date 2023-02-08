#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QStackedWidget>

class MainWindow;
class LoginDlg;

class MainWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    MainWindow *window;
    LoginDlg *login;

public slots:
    void switchStack(int);
};

#endif // MAINWIDGET_H
