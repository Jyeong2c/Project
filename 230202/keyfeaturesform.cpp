#include "keyfeaturesform.h"
#include "ui_keyfeaturesform.h"
//#include "view.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>
#include <QModelIndex>

KeyFeaturesForm::KeyFeaturesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyFeaturesForm)
{
    ui->setupUi(this);

    ui->horizontalSpacer->changeSize(20,10);
    ui->horizontalSpacer_2->changeSize(20,10);

    loadData();
//    keepCheck();
}

void KeyFeaturesForm::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "keyConnection");    // 데이터베이스를 만듦
    db.setDatabaseName("keyDBlist.db");                                         // DB이름 : keyDBlist
    if(db.open()) {                                                             // DB를 열 때
        QSqlQuery query(db);                                                    // DB에 커리생성

        /* 테이블을 생성하고 0~3열까지 헤더 이름을 지정해줍니다. */
        query.exec("CREATE TABLE IF NOT EXISTS KeyDB(id VARCHAR(20) Primary Key,"
                   "c1 VARCHAR(20), c2 VARCHAR(20), c3 VARCHAR(20), c4 VARCHAR(20), c5 VARCHAR(20),"
                   "c6 VARCHAR(20), c7 VARCHAR(20), c8 VARCHAR(20), c9 VARCHAR(20), c10 VARCHAR(20),"
                   "c11 VARCHAR(20), c12 VARCHAR(20), c13 VARCHAR(20), c14 VARCHAR(20), c15 VARCHAR(20),"
                   "c16 VARCHAR(20), c17 VARCHAR(20), c18 VARCHAR(20), c19 VARCHAR(20), c20 VARCHAR(20),"
                   "c21 VARCHAR(20), c22 VARCHAR(20), c23 VARCHAR(20), c24 VARCHAR(20));");
        keyModel = new QSqlTableModel(this, db);
        keyModel->setTable("KeyDB");                                            // 테이블명 : KeyDB
        keyModel->select();                                                     // keyModel을 재정렬
        /* 제품 tableView의 0~24열까지 헤더 이름을 지정 */
        keyModel->setHeaderData(0, Qt::Horizontal, tr("DoctorID"));
        keyModel->setHeaderData(1, Qt::Horizontal, tr("c1"));
        keyModel->setHeaderData(2, Qt::Horizontal, tr("c2"));
        keyModel->setHeaderData(3, Qt::Horizontal, tr("c3"));
        keyModel->setHeaderData(4, Qt::Horizontal, tr("c4"));
        keyModel->setHeaderData(5, Qt::Horizontal, tr("c5"));
        keyModel->setHeaderData(6, Qt::Horizontal, tr("c6"));
        keyModel->setHeaderData(7, Qt::Horizontal, tr("c7"));
        keyModel->setHeaderData(8, Qt::Horizontal, tr("c8"));
        keyModel->setHeaderData(9, Qt::Horizontal, tr("c9"));
        keyModel->setHeaderData(10, Qt::Horizontal, tr("c10"));
        keyModel->setHeaderData(11, Qt::Horizontal, tr("c11"));
        keyModel->setHeaderData(12, Qt::Horizontal, tr("c12"));
        keyModel->setHeaderData(13, Qt::Horizontal, tr("c13"));
        keyModel->setHeaderData(14, Qt::Horizontal, tr("c14"));
        keyModel->setHeaderData(15, Qt::Horizontal, tr("c15"));
        keyModel->setHeaderData(16, Qt::Horizontal, tr("c16"));
        keyModel->setHeaderData(17, Qt::Horizontal, tr("c17"));
        keyModel->setHeaderData(18, Qt::Horizontal, tr("c18"));
        keyModel->setHeaderData(19, Qt::Horizontal, tr("c19"));
        keyModel->setHeaderData(20, Qt::Horizontal, tr("c20"));
        keyModel->setHeaderData(21, Qt::Horizontal, tr("c21"));
        keyModel->setHeaderData(22, Qt::Horizontal, tr("c22"));
        keyModel->setHeaderData(23, Qt::Horizontal, tr("c23"));
        keyModel->setHeaderData(24, Qt::Horizontal, tr("c24"));

        ui->tableView->setModel(keyModel);                                      // tableView에 지정해준 헤더 이름을 가진 모델을 보여줍니다.
//        ui->tableView->hide();
    }
}

KeyFeaturesForm::~KeyFeaturesForm()
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("keyConnection");
    if(db.isOpen()) {                           // DB를 열 때
        keyModel->submitAll();               // 보류 중인 모든 변경 사항을 제출하고 성공 시 true를 반환합니다.
        delete keyModel;                     // keyModel 삭제
        db.commit();                            // DB 트랜잭션을 데이터베이스에 커밋합니다. 작업이 성공하면 true를 반환합니다. 그렇지 않으면 false를 반환합니다.
        db.close();                             // DB 닫음
    }

}


void KeyFeaturesForm::on_savePushButton_clicked()
{
    if(DID == "osstem1") {
//        insertInfo("osstem1");
//        updateInfo("osstem1");

        updateInfo();
        qDebug() << "osstem1이 호출됩니다.";
    }
    else if(DID == "osstem2") {
//        insertInfo("osstem2");
//        updateInfo("osstem2");

        updateInfo();
        qDebug() << "osstem2가 호출됩니다.";
    }
}


void KeyFeaturesForm::insertInfo(QString DoctorID)
{
    int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12;
    int c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24;

    c1 = ui->zoomInCheckBox->isChecked();
    c2 = ui->zoomOutCheckBox->isChecked();
    c3 = ui->rightRotateCheckBox->isChecked();
    c4 = ui->leftRotateCheckBox->isChecked();
    c5 = ui->brushCheckBox->isChecked();
    c6 = ui->rectangleCheckBox->isChecked();
    c7 = ui->triangleCheckBox->isChecked();
    c8 = ui->ellipseCheckBox->isChecked();
    c9 = ui->moveShapeCheckBox->isChecked();
    c10 = ui->blendingCheckBox->isChecked();
    c11 = ui->lengthMeasurementCheckBox->isChecked();
    c12 = ui->angleMeasurementCheckBox->isChecked();
    c13 = ui->brightnessCheckBox->isChecked();
    c14 = ui->darknessCheckBox->isChecked();
    c15 = ui->sharpenCheckBox->isChecked();
    c16 = ui->contrastCheckBox->isChecked();
    c17 = ui->inversionCheckBox->isChecked();
    c18 = ui->horizontalFlipCheckBox->isChecked();
    c19 = ui->verticalFlipCheckBox->isChecked();
    c20 = ui->implantCheckBox->isChecked();
    c21 = ui->sourceSizeCheckBox->isChecked();
    c22 = ui->layoutImageClearCheckBox->isChecked();
    c23 = ui->brushClearCheckBox->isChecked();
    c24 = ui->imageProcessingClearCheckBox->isChecked();

    QSqlDatabase db = QSqlDatabase::database("keyConnection");
    if(db.isOpen() && DoctorID.length()) {                              // DB를 열면
        QSqlQuery query(keyModel->database());
        query.prepare("INSERT INTO KeyDB VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");                // KeyDB 테이블에 입력한 정보를 삽입합니다.
        query.bindValue(0, DoctorID);                                   // 0열: Doctor ID
        query.bindValue(1, c1);                                         // 1열: 고객 이름
        query.bindValue(2, c2);
        query.bindValue(3, c3);
        query.bindValue(4, c4);
        query.bindValue(5, c5);
        query.bindValue(6, c6);
        query.bindValue(7, c7);
        query.bindValue(8, c8);
        query.bindValue(9, c9);
        query.bindValue(10, c10);
        query.bindValue(11, c11);
        query.bindValue(12, c12);
        query.bindValue(13, c13);
        query.bindValue(14, c14);
        query.bindValue(15, c15);
        query.bindValue(16, c16);
        query.bindValue(17, c17);
        query.bindValue(18, c18);
        query.bindValue(19, c19);
        query.bindValue(20, c20);
        query.bindValue(21, c21);
        query.bindValue(22, c22);
        query.bindValue(23, c23);
        query.bindValue(24, c24);
        query.exec();                                                   // 쿼리 실행
        keyModel->select();                                             // keyModel을 재정렬
    }
}

//void KeyFeaturesForm::updateInfo(QString DoctorID)
void KeyFeaturesForm::updateInfo()
{
    QModelIndexList indexes = keyModel->match(keyModel->index(0,0), Qt::EditRole,
                                              DID, -1, Qt::MatchFlags(Qt::MatchContains|Qt::MatchRecursive));
    QModelIndex ix= indexes.first();

        int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12;
        int c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24;

        c1 = ui->zoomInCheckBox->isChecked();
        c2 = ui->zoomOutCheckBox->isChecked();
        c3 = ui->rightRotateCheckBox->isChecked();
        c4 = ui->leftRotateCheckBox->isChecked();
        c5 = ui->brushCheckBox->isChecked();
        c6 = ui->rectangleCheckBox->isChecked();
        c7 = ui->triangleCheckBox->isChecked();
        c8 = ui->ellipseCheckBox->isChecked();
        c9 = ui->moveShapeCheckBox->isChecked();
        c10 = ui->blendingCheckBox->isChecked();
        c11 = ui->lengthMeasurementCheckBox->isChecked();
        c12 = ui->angleMeasurementCheckBox->isChecked();
        c13 = ui->brightnessCheckBox->isChecked();
        c14 = ui->darknessCheckBox->isChecked();
        c15 = ui->sharpenCheckBox->isChecked();
        c16 = ui->contrastCheckBox->isChecked();
        c17 = ui->inversionCheckBox->isChecked();
        c18 = ui->horizontalFlipCheckBox->isChecked();
        c19 = ui->verticalFlipCheckBox->isChecked();
        c20 = ui->implantCheckBox->isChecked();
        c21 = ui->sourceSizeCheckBox->isChecked();
        c22 = ui->layoutImageClearCheckBox->isChecked();
        c23 = ui->brushClearCheckBox->isChecked();
        c24 = ui->imageProcessingClearCheckBox->isChecked();

        keyModel->setData(ix.siblingAtColumn(1), c1);        // 선택된 행의 1열을 clientName으로 변경
        keyModel->setData(ix.siblingAtColumn(2), c2);       // 선택된 행의 2열을 phoneNumber으로 변경
        keyModel->setData(ix.siblingAtColumn(3), c3);           // 선택된 행의 3열을 address으로 변경
        keyModel->setData(ix.siblingAtColumn(4), c4);             // 선택된 행의 4열을 email으로 변경
        keyModel->setData(ix.siblingAtColumn(5), c5);        // 선택된 행의 1열을 clientName으로 변경
        keyModel->setData(ix.siblingAtColumn(6), c6);       // 선택된 행의 2열을 phoneNumber으로 변경
        keyModel->setData(ix.siblingAtColumn(7), c7);           // 선택된 행의 3열을 address으로 변경
        keyModel->setData(ix.siblingAtColumn(8), c8);
        keyModel->setData(ix.siblingAtColumn(9), c9);        // 선택된 행의 1열을 clientName으로 변경
        keyModel->setData(ix.siblingAtColumn(10), c10);       // 선택된 행의 2열을 phoneNumber으로 변경
        keyModel->setData(ix.siblingAtColumn(11), c11);           // 선택된 행의 3열을 address으로 변경
        keyModel->setData(ix.siblingAtColumn(12), c12);
        keyModel->setData(ix.siblingAtColumn(13), c13);        // 선택된 행의 1열을 clientName으로 변경
        keyModel->setData(ix.siblingAtColumn(14), c14);       // 선택된 행의 2열을 phoneNumber으로 변경
        keyModel->setData(ix.siblingAtColumn(15), c15);           // 선택된 행의 3열을 address으로 변경
        keyModel->setData(ix.siblingAtColumn(16), c16);
        keyModel->setData(ix.siblingAtColumn(17), c17);        // 선택된 행의 1열을 clientName으로 변경
        keyModel->setData(ix.siblingAtColumn(18), c18);       // 선택된 행의 2열을 phoneNumber으로 변경
        keyModel->setData(ix.siblingAtColumn(19), c19);           // 선택된 행의 3열을 address으로 변경
        keyModel->setData(ix.siblingAtColumn(20), c20);
        keyModel->setData(ix.siblingAtColumn(21), c21);        // 선택된 행의 1열을 clientName으로 변경
        keyModel->setData(ix.siblingAtColumn(22), c22);       // 선택된 행의 2열을 phoneNumber으로 변경
        keyModel->setData(ix.siblingAtColumn(23), c23);           // 선택된 행의 3열을 address으로 변경
        keyModel->setData(ix.siblingAtColumn(24), c24);
        keyModel->submit();                                          // 변경 사항 제출
        keyModel->select();

        emit sig_keyCheck(c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13,
                          c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24);

}


void KeyFeaturesForm::slot_doctorInfo(QString DoctorID)
{
    DID = DoctorID;
    qDebug() << "KeyFeaturesForm에 전달됨" << DoctorID;
}

void KeyFeaturesForm::keepCheck()
{
////    if(keyModel != nullptr) {
//        QModelIndexList indexes = keyModel->match(keyModel->index(0,0), Qt::EditRole,
//                                                  DID, -1, Qt::MatchFlags(Qt::MatchContains|Qt::MatchRecursive));
//        QModelIndex ix= indexes.first();

//        int c1 = keyModel->data(ix.siblingAtColumn(1)).toInt();
//        int c2 = keyModel->data(ix.siblingAtColumn(2)).toInt();
//        int c3 = keyModel->data(ix.siblingAtColumn(3)).toInt();
//        int c4 = keyModel->data(ix.siblingAtColumn(4)).toInt();
//        int c5 = keyModel->data(ix.siblingAtColumn(5)).toInt();
//        int c6 = keyModel->data(ix.siblingAtColumn(6)).toInt();
//        int c7 = keyModel->data(ix.siblingAtColumn(7)).toInt();
//        int c8 = keyModel->data(ix.siblingAtColumn(8)).toInt();
//        int c9 = keyModel->data(ix.siblingAtColumn(9)).toInt();
//        int c10 = keyModel->data(ix.siblingAtColumn(10)).toInt();
//        int c11 = keyModel->data(ix.siblingAtColumn(11)).toInt();
//        int c12 = keyModel->data(ix.siblingAtColumn(12)).toInt();
//        int c13 = keyModel->data(ix.siblingAtColumn(13)).toInt();
//        int c14 = keyModel->data(ix.siblingAtColumn(14)).toInt();
//        int c15 = keyModel->data(ix.siblingAtColumn(15)).toInt();
//        int c16 = keyModel->data(ix.siblingAtColumn(16)).toInt();
//        int c17 = keyModel->data(ix.siblingAtColumn(17)).toInt();
//        int c18 = keyModel->data(ix.siblingAtColumn(18)).toInt();
//        int c19 = keyModel->data(ix.siblingAtColumn(19)).toInt();
//        int c20 = keyModel->data(ix.siblingAtColumn(20)).toInt();
//        int c21 = keyModel->data(ix.siblingAtColumn(21)).toInt();
//        int c22 = keyModel->data(ix.siblingAtColumn(22)).toInt();
//        int c23 = keyModel->data(ix.siblingAtColumn(23)).toInt();
//        int c24 = keyModel->data(ix.siblingAtColumn(24)).toInt();

//    QStringList checkStrings;
//    checkStrings << DID << QString::number(c1) << QString::number(c2) << QString::number(c3) << QString::number(c4)
//                 << QString::number(c5) << QString::number(c6) << QString::number(c7) << QString::number(c8) << QString::number(c9)
//                 << QString::number(c10) << QString::number(c11) << QString::number(c12) << QString::number(c13) << QString::number(c14)
//                 << QString::number(c15) << QString::number(c16) << QString::number(c17) << QString::number(c18) << QString::number(c19)
//                 << QString::number(c20) << QString::number(c21) << QString::number(c22) << QString::number(c23) << QString::number(c24);

//    emit sig_checkShow(checkStrings);

//        if(c1 == 1)
//            ui->zoomInCheckBox->setChecked(true);
//        if(c2 == 1)
//            ui->zoomOutCheckBox->setChecked(true);
//        if(c3 == 1)
//            ui->rightRotateCheckBox->setChecked(true);
//        if(c4 == 1)
//            ui->leftRotateCheckBox->setChecked(true);
//        if(c5 == 1)
//            ui->brushCheckBox->setChecked(true);
//        if(c6 == 1)
//            ui->rectangleCheckBox->setChecked(true);
//        if(c7 == 1)
//            ui->triangleCheckBox->setChecked(true);
//        if(c8 == 1)
//            ui->ellipseCheckBox->setChecked(true);
//        if(c9 == 1)
//            ui->moveShapeCheckBox->setChecked(true);
//        if(c10 == 1)
//            ui->blendingCheckBox->setChecked(true);
//        if(c11 == 1)
//            ui->lengthMeasurementCheckBox->setChecked(true);
//        if(c12 == 1)
//            ui->angleMeasurementCheckBox->setChecked(true);
//        if(c13 == 1)
//            ui->brightnessCheckBox->setChecked(true);
//        if(c14 == 1)
//            ui->darknessCheckBox->setChecked(true);
//        if(c15 == 1)
//            ui->sharpenCheckBox->setChecked(true);
//        if(c16 == 1)
//            ui->contrastCheckBox->setChecked(true);
//        if(c17 == 1)
//            ui->inversionCheckBox->setChecked(true);
//        if(c18 == 1)
//            ui->horizontalFlipCheckBox->setChecked(true);
//        if(c19 == 1)
//            ui->verticalFlipCheckBox->setChecked(true);
//        if(c20 == 1)
//            ui->implantCheckBox->setChecked(true);
//        if(c21 == 1)
//            ui->sourceSizeCheckBox->setChecked(true);
//        if(c22 == 1)
//            ui->layoutImageClearCheckBox->setChecked(true);
//        if(c23 == 1)
//            ui->brushClearCheckBox->setChecked(true);
//        if(c24 == 1)
//            ui->imageProcessingClearCheckBox->setChecked(true);
}


