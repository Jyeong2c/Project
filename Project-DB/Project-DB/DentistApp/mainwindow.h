#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QSqlQuery;
class QSqlTableModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void patientLoad();

private:
    Ui::MainWindow *ui;
    QSqlQuery *patientQuery;
    QSqlTableModel *patientQueryModel;
};
#endif // MAINWINDOW_H
