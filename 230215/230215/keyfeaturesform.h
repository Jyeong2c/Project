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

    QString DID;    // 의사 ID 저장
    bool save = false;

    void loadData();    // Key Features DB Table 생성하는 함수


public slots:
    void insertInfo(QString DoctorID);  // DB에 정보를 입력하는 슬롯
    void updateInfo();  // savePushButton을 눌렀을 때 DB 데이터를 변경해주는 슬롯
    void slot_doctorInfo(QString DoctorID); // 로그인할 때 실행되는 슬롯
    void slot_checkClear(); // 로그아웃할 때 실행되는 슬롯
    void slot_checkUpdate();    // 로그인할 때 실행되는 슬롯


private slots:
    void on_savePushButton_clicked();


private:
    Ui::KeyFeaturesForm *ui;

    QSqlTableModel* keyModel;


signals:
    void sig_keyCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                      int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    void sig_logCheck(int, int, int, int, int, int, int, int, int, int, int, int,
                      int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);

    void sig_save(bool);
};

#endif // KEYFEATURESFORM_H
