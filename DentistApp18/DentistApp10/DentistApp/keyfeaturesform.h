#ifndef KEYFEATURESFORM_H
#define KEYFEATURESFORM_H

#include "qsettings.h"
#include <QWidget>

class QSqlQuery;
class QSqlTableModel;


namespace Ui {
class KeyFeaturesForm;
}

class KeyFeaturesForm : public QWidget
{
    Q_OBJECT

public:
    explicit KeyFeaturesForm(QWidget *parent = nullptr);
    ~KeyFeaturesForm();


    void loadData();
    void checkState();


public slots:
    void test(QString DoctorID);



private slots:
    void on_savePushButton_clicked();



private:
    Ui::KeyFeaturesForm *ui;

    QSqlTableModel* keyModel;                            // 주요 기능 커리모델




};

#endif // KEYFEATURESFORM_H
