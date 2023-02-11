#ifndef KEYFEATURESFORM_H
#define KEYFEATURESFORM_H


#include <QWidget>

class QSqlQuery;
class QSqlTableModel;
class QModelIndex;


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

    bool save = false;


public slots:
    void slot_doctorInfo(QString DoctorID);

    void insertInfo(QString DoctorID);
//    void updateInfo(QString DoctorID);
    void updateInfo();
//    void keepCheck();

    void slot_checkClear();
    void slot_checkUpdate();


private slots:
    void on_savePushButton_clicked();




private:
    Ui::KeyFeaturesForm *ui;

    QSqlTableModel* keyModel;                            // 주요 기능 커리모델



signals:
//    void sig_checkShow(QStringList checkStrings);
    void sig_keyCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                      int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    void sig_logCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                      int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);

    void sig_save(bool);
};

#endif // KEYFEATURESFORM_H
