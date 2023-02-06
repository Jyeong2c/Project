#ifndef KEYFEATURESFORM_H
#define KEYFEATURESFORM_H


//#include "qabstractitemmodel.h"
#include <QWidget>

class QSqlQuery;
class QSqlTableModel;
class QModelIndex;

class View;


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

    QString DID;



public slots:
    void slot_doctorInfo(QString DoctorID);

    void insertInfo(QString DoctorID);
//    void updateInfo(QString DoctorID);
    void updateInfo();
    void keepCheck();




private slots:
    void on_savePushButton_clicked();




private:
    Ui::KeyFeaturesForm *ui;

    QSqlTableModel* keyModel;                            // 주요 기능 커리모델



signals:
    void sig_checkShow(QStringList checkStrings);
    void sig_keyCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                      int, int, int, int, int, int, int, int, int, int, int, int);

};

#endif // KEYFEATURESFORM_H
