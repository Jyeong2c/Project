#include "keyfeaturesform.h"
#include "ui_keyfeaturesform.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>

KeyFeaturesForm::KeyFeaturesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyFeaturesForm)
{
    ui->setupUi(this);

    ui->verticalSpacer->changeSize(10,10);
    ui->verticalSpacer_2->changeSize(10,10);
    ui->horizontalSpacer->changeSize(20,10);
    ui->horizontalSpacer_2->changeSize(20,10);

    loadData();
    checkState();

}

KeyFeaturesForm::~KeyFeaturesForm()
{
    delete ui;
}


void KeyFeaturesForm::checkState()
{
    QSettings CheckBox(QString("./CheckBox/%1.ini").arg("CheckStateFile"), QSettings::IniFormat);
    CheckBox.beginGroup("checking");
    ui->zoomInCheckBox->setChecked(CheckBox.value("checkState1").toBool());
    ui->zoomOutCheckBox->setChecked(CheckBox.value("checkState2").toBool());
    ui->leftRotateCheckBox->setChecked(CheckBox.value("checkState3").toBool());
    ui->rightRotateCheckBox->setChecked(CheckBox.value("checkState4").toBool());
    ui->brushCheckBox->setChecked(CheckBox.value("checkState5").toBool());
    ui->rectangleCheckBox->setChecked(CheckBox.value("checkState6").toBool());
    ui->triangleCheckBox->setChecked(CheckBox.value("checkState7").toBool());
    ui->ellipseCheckBox->setChecked(CheckBox.value("checkState8").toBool());
    ui->moveShapeCheckBox->setChecked(CheckBox.value("checkState9").toBool());
    ui->blendingCheckBox->setChecked(CheckBox.value("checkState10").toBool());
    ui->lengthMeasurementCheckBox->setChecked(CheckBox.value("checkState11").toBool());
    ui->angleMeasurementCheckBox->setChecked(CheckBox.value("checkState12").toBool());
    ui->brightnessCheckBox->setChecked(CheckBox.value("checkState13").toBool());
    ui->darknessCheckBox->setChecked(CheckBox.value("checkState14").toBool());
    ui->sharpenCheckBox->setChecked(CheckBox.value("checkState15").toBool());
    ui->contrastCheckBox->setChecked(CheckBox.value("checkState16").toBool());
    ui->inversionCheckBox->setChecked(CheckBox.value("checkState17").toBool());
    ui->horizontalFlipCheckBox->setChecked(CheckBox.value("checkState18").toBool());
    ui->verticalFlipCheckBox->setChecked(CheckBox.value("checkState19").toBool());
    ui->implantCheckBox->setChecked(CheckBox.value("checkState20").toBool());
    CheckBox.endGroup();
}


void KeyFeaturesForm::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "keyConnection");    // 데이터베이스를 만듦
    db.setDatabaseName("keyDBlist.db");                                         // DB이름 : keyDBlist
    if(db.open()) {                                                             // DB를 열 때
        QSqlQuery query(db);                                                    // DB에 커리생성

        /* 테이블을 생성하고 0~3열까지 헤더 이름을 지정해줍니다. */
        query.exec("CREATE TABLE IF NOT EXISTS KeyDB(id VARCHAR(20) Primary Key,"
                   "checking VARCHAR(20));");
        keyModel = new QSqlTableModel(this, db);
        keyModel->setTable("KeyDB");                                            // 테이블명 : KeyDB
        keyModel->select();                                                     // keyModel을 재정렬
        /* 제품 tableView의 0~1열까지 헤더 이름을 지정 */
        keyModel->setHeaderData(0, Qt::Horizontal, tr("DoctorID"));
        keyModel->setHeaderData(1, Qt::Horizontal, tr("Checking"));

        ui->tableView->setModel(keyModel);                                      // tableView에 지정해준 헤더 이름을 가진 모델을 보여줍니다.
    }
}



void KeyFeaturesForm::on_savePushButton_clicked()
{
#if 1
    QSettings CheckBox(QString("./CheckBox/%1.ini").arg("CheckStateFile"), QSettings::IniFormat);
    CheckBox.beginGroup("checking");
    CheckBox.setValue("checkState1", ui->zoomInCheckBox->isChecked());
    CheckBox.setValue("checkState2", ui->zoomOutCheckBox->isChecked());
    CheckBox.setValue("checkState3", ui->leftRotateCheckBox->isChecked());
    CheckBox.setValue("checkState4", ui->rightRotateCheckBox->isChecked());
    CheckBox.setValue("checkState5", ui->brushCheckBox->isChecked());
    CheckBox.setValue("checkState6", ui->rectangleCheckBox->isChecked());
    CheckBox.setValue("checkState7", ui->triangleCheckBox->isChecked());
    CheckBox.setValue("checkState8", ui->ellipseCheckBox->isChecked());
    CheckBox.setValue("checkState9", ui->moveShapeCheckBox->isChecked());
    CheckBox.setValue("checkState10", ui->blendingCheckBox->isChecked());
    CheckBox.setValue("checkState11", ui->lengthMeasurementCheckBox->isChecked());
    CheckBox.setValue("checkState12", ui->angleMeasurementCheckBox->isChecked());
    CheckBox.setValue("checkState13", ui->brightnessCheckBox->isChecked());
    CheckBox.setValue("checkState14", ui->darknessCheckBox->isChecked());
    CheckBox.setValue("checkState15", ui->sharpenCheckBox->isChecked());
    CheckBox.setValue("checkState16", ui->contrastCheckBox->isChecked());
    CheckBox.setValue("checkState17", ui->inversionCheckBox->isChecked());
    CheckBox.setValue("checkState18", ui->horizontalFlipCheckBox->isChecked());
    CheckBox.setValue("checkState19", ui->verticalFlipCheckBox->isChecked());
    CheckBox.setValue("checkState20", ui->implantCheckBox->isChecked());
    CheckBox.endGroup();

#else
    QString DoctorID;
    bool checking;

    checking = ui->zoomInCheckBox->isChecked();

    QSqlDatabase db = QSqlDatabase::database("keyConnection");
    if(db.isOpen() && DoctorID.length()) {                          // DB를 열면
        QSqlQuery query(keyModel->database());
        query.prepare("INSERT INTO KeyDB VALUES(?, ?)");            // ClientDB 테이블에 입력한 정보를 삽입합니다.
        query.bindValue(0, DoctorID);                                   // 0열: 고객 ID
        query.bindValue(1, checking);                                   // 1열: 고객 이름
        query.exec();                                                   // 쿼리 실행

        keyModel->select();                                          // clientModel을 재정렬
    }
#endif
}




void KeyFeaturesForm::test(QString DoctorID)
{

}

