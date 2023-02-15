#include "keyfeaturesform.h"
#include "ui_keyfeaturesform.h"

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
}

/* Key Features DB Table 생성 */
void KeyFeaturesForm::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "keyConnection");    // DB 만듦
    db.setDatabaseName("keyDBlist.db"); // DB이름 : keyDBlist
    if(db.open()) { // DB를 열 때
        QSqlQuery query(db);    // DB에 커리생성

        /* 테이블을 생성하고 0~27열까지 헤더 이름을 지정해줍니다. */
        query.exec("CREATE TABLE IF NOT EXISTS KeyDB(id VARCHAR(20) Primary Key,"
                   "c1 VARCHAR(20), c2 VARCHAR(20), c3 VARCHAR(20), c4 VARCHAR(20), c5 VARCHAR(20),"
                   "c6 VARCHAR(20), c7 VARCHAR(20), c8 VARCHAR(20), c9 VARCHAR(20), c10 VARCHAR(20),"
                   "c11 VARCHAR(20), c12 VARCHAR(20), c13 VARCHAR(20), c14 VARCHAR(20), c15 VARCHAR(20),"
                   "c16 VARCHAR(20), c17 VARCHAR(20), c18 VARCHAR(20), c19 VARCHAR(20), c20 VARCHAR(20),"
                   "c21 VARCHAR(20), c22 VARCHAR(20), c23 VARCHAR(20), c24 VARCHAR(20), c25 VARCHAR(20), c26 VARCHAR(20), c27 VARCHAR(20));");
        keyModel = new QSqlTableModel(this, db);    // keyModel 할당
        keyModel->setTable("KeyDB");    // 테이블명 : KeyDB
        keyModel->select(); // keyModel을 재정렬
        /* Key Features tableView의 0~24열까지 헤더 이름을 지정 */
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
        keyModel->setHeaderData(25, Qt::Horizontal, tr("c25"));
        keyModel->setHeaderData(26, Qt::Horizontal, tr("c26"));
        keyModel->setHeaderData(27, Qt::Horizontal, tr("c27"));

        ui->tableView->setModel(keyModel);  // tableView에 지정해준 헤더 이름을 가진 모델을 보여줌
        ui->tableView->hide();  // tableView 숨김
    }
}

KeyFeaturesForm::~KeyFeaturesForm()
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("keyConnection");
    if(db.isOpen()) {   // DB를 열 때
        keyModel->submitAll();  // 보류 중인 모든 변경 사항을 제출하고 성공 시 true를 반환
        delete keyModel;    // keyModel 삭제
        db.commit();    // DB에 커밋, 작업이 성공하면 true 그렇지 않으면 false를 반환
        db.close(); // DB 닫음
    }
}

/* savePushButton을 눌렀을 때 실행되는 슬롯 */
void KeyFeaturesForm::on_savePushButton_clicked()
{
    save = true;

    if(DID == "osstem1") {
        updateInfo();
    }
    else if(DID == "osstem2") {
        updateInfo();
    }
    else if(DID == "osstem3") {
        updateInfo();
    }
    emit sig_save(save);
}

/* DB에 정보를 입력하는 슬롯 */
void KeyFeaturesForm::insertInfo(QString DoctorID)
{
    int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12;
    int c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27;

    c1 = ui->zoomInCheckBox->isChecked();   // 확대 기능 체크 여부
    c2 = ui->zoomOutCheckBox->isChecked();  // 축소 기능 체크 여부
    c3 = ui->rightRotateCheckBox->isChecked();  // 90°회전 기능 체크 여부
    c4 = ui->leftRotateCheckBox->isChecked();   // -90°회전 기능 체크 여부
    c5 = ui->brushCheckBox->isChecked();    // 브러쉬 기능 체크 여부
    c6 = ui->rectangleCheckBox->isChecked();    // 사각형 기능 체크 여부
    c7 = ui->triangleCheckBox->isChecked(); // 삼각형 기능 체크 여부
    c8 = ui->ellipseCheckBox->isChecked();  //원형 기능 체크 여부
    c9 = ui->cursorCheckBox->isChecked();   // 커서 기능 체크 여부
    c10 = ui->blendingCheckBox->isChecked();    // 블랜딩 기능 체크 여부
    c11 = ui->lengthMeasurementCheckBox->isChecked();   // 길이 측정 기능 체크 여부
    c12 = ui->angleMeasurementCheckBox->isChecked();    // 각도 측정 기능 체크 여부
    c13 = ui->brightnessCheckBox->isChecked();  // 밝기 기능 체크 여부
    c14 = ui->darknessCheckBox->isChecked();    // 어둡기 기능 체크 여부
    c15 = ui->sharpenCheckBox->isChecked(); // 선명화 기능 체크 여부
    c16 = ui->contrastCheckBox->isChecked();    // 명암 기능 체크 여부
    c17 = ui->inversionCheckBox->isChecked();   // 대비 기능 체크 여부
    c18 = ui->horizontalFlipCheckBox->isChecked();  // 수평 반전 기능 체크 여부
    c19 = ui->verticalFlipCheckBox->isChecked();    // 수직 반전 기능 체크 여부
    c20 = ui->implantCheckBox->isChecked(); // 임플란트 기능 체크 여부
    c21 = ui->sourceSizeCheckBox->isChecked();  // 원본 크기 기능 체크 여부
    c22 = ui->layoutImageClearCheckBox->isChecked();    // 레이아웃 초기화 기능 체크 여부
    c23 = ui->drawClearCheckBox->isChecked();   // Draw 초기화 기능 체크 여부
    c24 = ui->implantClearCheckBox->isChecked();    // 임플란트 초기화 기능 체크 여부
    c25 = ui->imageProcessingClearCheckBox->isChecked();    // 이미지 프로세싱 초기화 기능 체크 여부
    c26 = ui->arrowCheckBox->isChecked();   // 화살표 기능 체크 여부
    c27 = ui->lineCheckBox->isChecked();    // 선 기능 체크 여부

    QSqlDatabase db = QSqlDatabase::database("keyConnection");
    if(db.isOpen() && DoctorID.length()) {
        QSqlQuery query(keyModel->database());
        query.prepare("INSERT INTO KeyDB VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");  // KeyDB 테이블에 입력한 정보를 삽입
        query.bindValue(0, DoctorID);   // 0열: Doctor ID
        query.bindValue(1, c1);
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
        query.bindValue(25, c25);
        query.bindValue(26, c26);
        query.bindValue(27, c27);
        query.exec();   // 쿼리 실행
        keyModel->select(); // keyModel을 재정렬
    }
}

/* savePushButton을 눌렀을 때 DB 데이터를 변경해주는 슬롯 */
void KeyFeaturesForm::updateInfo()
{
    /* 0열에 있는 의사 ID와 일치하는 행을 찾음 */
    QModelIndexList indexes = keyModel->match(keyModel->index(0,0), Qt::EditRole,
                                              DID, -1, Qt::MatchFlags(Qt::MatchContains|Qt::MatchRecursive));
    QModelIndex ix = indexes.first();    // 일치하는 첫 번째 데이터

        int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12;
        int c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27;

        c1 = ui->zoomInCheckBox->isChecked();
        c2 = ui->zoomOutCheckBox->isChecked();
        c3 = ui->rightRotateCheckBox->isChecked();
        c4 = ui->leftRotateCheckBox->isChecked();
        c5 = ui->brushCheckBox->isChecked();
        c6 = ui->rectangleCheckBox->isChecked();
        c7 = ui->triangleCheckBox->isChecked();
        c8 = ui->ellipseCheckBox->isChecked();
        c9 = ui->cursorCheckBox->isChecked();
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
        c23 = ui->drawClearCheckBox->isChecked();
        c24 = ui->implantClearCheckBox->isChecked();
        c25 = ui->imageProcessingClearCheckBox->isChecked();
        c26 = ui->arrowCheckBox->isChecked();
        c27 = ui->lineCheckBox->isChecked();

        keyModel->setData(ix.siblingAtColumn(1), c1);   // 1열을 c1의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(2), c2);   // 2열을 c2의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(3), c3);   // 3열을 c3의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(4), c4);   // 4열을 c4의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(5), c5);   // 5열을 c5의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(6), c6);   // 6열을 c6의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(7), c7);   // 7열을 c7의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(8), c8);   // 8열을 c8의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(9), c9);   // 9열을 c9의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(10), c10); // 10열을 c10의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(11), c11); // 11열을 c11의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(12), c12); // 12열을 c12의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(13), c13); // 13열을 c13의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(14), c14); // 14열을 c14의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(15), c15); // 15열을 c15의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(16), c16); // 16열을 c16의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(17), c17); // 17열을 c17의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(18), c18); // 18열을 c18의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(19), c19); // 19열을 c19의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(20), c20); // 20열을 c20의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(21), c21); // 21열을 c21의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(22), c22); // 22열을 c22의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(23), c23); // 23열을 c23의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(24), c24); // 24열을 c24의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(25), c25); // 25열을 c25의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(26), c26); // 26열을 c26의 체크 상태 값으로 변경
        keyModel->setData(ix.siblingAtColumn(27), c27); // 27열을 c27의 체크 상태 값으로 변경
        keyModel->submit(); // 변경 사항 제출
        keyModel->select(); // keyModel을 재정렬

        emit sig_keyCheck(c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13,
                          c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27);
}

/* 로그인할 때 실행되는 슬롯 */
void KeyFeaturesForm::slot_doctorInfo(QString DoctorID)
{
    DID = DoctorID; // 로그인할 때 입력한 의사 ID를 DID 멤버변수에 대입

    save = false;
    emit sig_save(save);
}

/* 로그아웃할 때 실행되는 슬롯 - 모든 기능들의 체크 상태를 false로 초기화 */
void KeyFeaturesForm::slot_checkClear()
{
    save = false;
    emit sig_save(save);

    ui->zoomInCheckBox->setChecked(false);
    ui->zoomOutCheckBox->setChecked(false);
    ui->rightRotateCheckBox->setChecked(false);
    ui->leftRotateCheckBox->setChecked(false);
    ui->brushCheckBox->setChecked(false);
    ui->rectangleCheckBox->setChecked(false);
    ui->triangleCheckBox->setChecked(false);
    ui->ellipseCheckBox->setChecked(false);
    ui->cursorCheckBox->setChecked(false);
    ui->blendingCheckBox->setChecked(false);
    ui->lengthMeasurementCheckBox->setChecked(false);
    ui->angleMeasurementCheckBox->setChecked(false);
    ui->brightnessCheckBox->setChecked(false);
    ui->darknessCheckBox->setChecked(false);
    ui->sharpenCheckBox->setChecked(false);
    ui->contrastCheckBox->setChecked(false);
    ui->inversionCheckBox->setChecked(false);
    ui->horizontalFlipCheckBox->setChecked(false);
    ui->verticalFlipCheckBox->setChecked(false);
    ui->implantCheckBox->setChecked(false);
    ui->sourceSizeCheckBox->setChecked(false);
    ui->layoutImageClearCheckBox->setChecked(false);
    ui->drawClearCheckBox->setChecked(false);
    ui->implantClearCheckBox->setChecked(false);
    ui->imageProcessingClearCheckBox->setChecked(false);
    ui->arrowCheckBox->setChecked(false);
    ui->lineCheckBox->setChecked(false);
}

/* 로그인할 때 실행되는 슬롯 */
void KeyFeaturesForm::slot_checkUpdate()
{
    QModelIndexList indexes = keyModel->match(keyModel->index(0,0), Qt::EditRole,
                                              DID, -1, Qt::MatchFlags(Qt::MatchContains|Qt::MatchRecursive));
    QModelIndex ix= indexes.first();

    int c1 = keyModel->data(ix.siblingAtColumn(1)).toInt();
    int c2 = keyModel->data(ix.siblingAtColumn(2)).toInt();
    int c3 = keyModel->data(ix.siblingAtColumn(3)).toInt();
    int c4 = keyModel->data(ix.siblingAtColumn(4)).toInt();
    int c5 = keyModel->data(ix.siblingAtColumn(5)).toInt();
    int c6 = keyModel->data(ix.siblingAtColumn(6)).toInt();
    int c7 = keyModel->data(ix.siblingAtColumn(7)).toInt();
    int c8 = keyModel->data(ix.siblingAtColumn(8)).toInt();
    int c9 = keyModel->data(ix.siblingAtColumn(9)).toInt();
    int c10 = keyModel->data(ix.siblingAtColumn(10)).toInt();
    int c11 = keyModel->data(ix.siblingAtColumn(11)).toInt();
    int c12 = keyModel->data(ix.siblingAtColumn(12)).toInt();
    int c13 = keyModel->data(ix.siblingAtColumn(13)).toInt();
    int c14 = keyModel->data(ix.siblingAtColumn(14)).toInt();
    int c15 = keyModel->data(ix.siblingAtColumn(15)).toInt();
    int c16 = keyModel->data(ix.siblingAtColumn(16)).toInt();
    int c17 = keyModel->data(ix.siblingAtColumn(17)).toInt();
    int c18 = keyModel->data(ix.siblingAtColumn(18)).toInt();
    int c19 = keyModel->data(ix.siblingAtColumn(19)).toInt();
    int c20 = keyModel->data(ix.siblingAtColumn(20)).toInt();
    int c21 = keyModel->data(ix.siblingAtColumn(21)).toInt();
    int c22 = keyModel->data(ix.siblingAtColumn(22)).toInt();
    int c23 = keyModel->data(ix.siblingAtColumn(23)).toInt();
    int c24 = keyModel->data(ix.siblingAtColumn(24)).toInt();
    int c25 = keyModel->data(ix.siblingAtColumn(25)).toInt();
    int c26 = keyModel->data(ix.siblingAtColumn(26)).toInt();
    int c27 = keyModel->data(ix.siblingAtColumn(27)).toInt();

    /* 로그인할 때 DB에 저장된 기능의 값이 1이면 현재 체크 상태를 true로 변경 */
    if(c1 == 1)
        ui->zoomInCheckBox->setChecked(true);
    if(c2 == 1)
        ui->zoomOutCheckBox->setChecked(true);
    if(c3 == 1)
        ui->rightRotateCheckBox->setChecked(true);
    if(c4 == 1)
        ui->leftRotateCheckBox->setChecked(true);
    if(c5 == 1)
        ui->brushCheckBox->setChecked(true);
    if(c6 == 1)
        ui->rectangleCheckBox->setChecked(true);
    if(c7 == 1)
        ui->triangleCheckBox->setChecked(true);
    if(c8 == 1)
        ui->ellipseCheckBox->setChecked(true);
    if(c9 == 1)
        ui->cursorCheckBox->setChecked(true);
    if(c10 == 1)
        ui->blendingCheckBox->setChecked(true);
    if(c11 == 1)
        ui->lengthMeasurementCheckBox->setChecked(true);
    if(c12 == 1)
        ui->angleMeasurementCheckBox->setChecked(true);
    if(c13 == 1)
        ui->brightnessCheckBox->setChecked(true);
    if(c14 == 1)
        ui->darknessCheckBox->setChecked(true);
    if(c15 == 1)
        ui->sharpenCheckBox->setChecked(true);
    if(c16 == 1)
        ui->contrastCheckBox->setChecked(true);
    if(c17 == 1)
        ui->inversionCheckBox->setChecked(true);
    if(c18 == 1)
        ui->horizontalFlipCheckBox->setChecked(true);
    if(c19 == 1)
        ui->verticalFlipCheckBox->setChecked(true);
    if(c20 == 1)
        ui->implantCheckBox->setChecked(true);
    if(c21 == 1)
        ui->sourceSizeCheckBox->setChecked(true);
    if(c22 == 1)
        ui->layoutImageClearCheckBox->setChecked(true);
    if(c23 == 1)
        ui->drawClearCheckBox->setChecked(true);
    if(c24 == 1)
        ui->implantClearCheckBox->setChecked(true);
    if(c25 == 1)
        ui->imageProcessingClearCheckBox->setChecked(true);
    if(c26 == 1)
        ui->arrowCheckBox->setChecked(true);
    if(c27 == 1)
        ui->lineCheckBox->setChecked(true);

    emit sig_logCheck(c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13,
                      c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27);
}




