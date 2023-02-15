#include "teethform.h"
#include "ui_teethform.h"


#include <QLabel>

TeethForm::TeethForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeethForm)
{
    ui->setupUi(this);

    this->setStyleSheet("background:rgb(0, 0, 0)");
    this->resize(500, 300);
}

TeethForm::~TeethForm()
{
    delete ui;
}

/* Top Left Teeth images - 치아 11번 ~ 17번 */
void TeethForm::on_toolButton_11_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/11.png"));
    QPixmap* teeth11 = new QPixmap(imagePath);
    emit sig_11(teeth11);   // 치아 11번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_12_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/12.png"));
    QPixmap* teeth12 = new QPixmap(imagePath);
    emit sig_12(teeth12);   // 치아 12번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_13_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/13.png"));
    QPixmap* teeth13 = new QPixmap(imagePath);
    emit sig_13(teeth13);   // 치아 13번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_14_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/14.png"));
    QPixmap* teeth14 = new QPixmap(imagePath);
    emit sig_14(teeth14);   // 치아 14번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_15_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/15.png"));
    QPixmap* teeth15 = new QPixmap(imagePath);
    emit sig_15(teeth15);   // 치아 15번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_16_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/16.png"));
    QPixmap* teeth16 = new QPixmap(imagePath);
    emit sig_16(teeth16);   // 치아 16번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_17_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topLeft/17.png"));
    QPixmap* teeth17 = new QPixmap(imagePath);
    emit sig_17(teeth17);   // 치아 17번 이미지를 시그널로 전달
    close();
}


/* Top Right Teeth images - 치아 21번 ~ 27번 */
void TeethForm::on_toolButton_21_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/21.png"));
    QPixmap* teeth21 = new QPixmap(imagePath);
    emit sig_21(teeth21);   // 치아 21번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_22_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/22.png"));
    QPixmap* teeth22 = new QPixmap(imagePath);
    emit sig_22(teeth22);   // 치아 22번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_23_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/23.png"));
    QPixmap* teeth23 = new QPixmap(imagePath);
    emit sig_23(teeth23);   // 치아 23번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_24_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/24.png"));
    QPixmap* teeth24 = new QPixmap(imagePath);
    emit sig_24(teeth24);   // 치아 24번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_25_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/25.png"));
    QPixmap* teeth25 = new QPixmap(imagePath);
    emit sig_25(teeth25);   // 치아 25번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_26_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/26.png"));
    QPixmap* teeth26 = new QPixmap(imagePath);
    emit sig_26(teeth26);   // 치아 26번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_27_clicked()
{
    QString imagePath(tr(":/teeth/teeth/topRight/27.png"));
    QPixmap* teeth27 = new QPixmap(imagePath);
    emit sig_27(teeth27);   // 치아 27번 이미지를 시그널로 전달
    close();
}

/* Bottom Right images - 치아 31번 ~ 37번 */
void TeethForm::on_toolButton_31_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/31.png"));
    QPixmap* teeth31 = new QPixmap(imagePath);
    emit sig_31(teeth31);   // 치아 31번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_32_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/32.png"));
    QPixmap* teeth32 = new QPixmap(imagePath);
    emit sig_32(teeth32);   // 치아 32번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_33_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/33.png"));
    QPixmap* teeth33 = new QPixmap(imagePath);
    emit sig_33(teeth33);   // 치아 33번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_34_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/34.png"));
    QPixmap* teeth34 = new QPixmap(imagePath);
    emit sig_34(teeth34);   // 치아 34번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_35_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/35.png"));
    QPixmap* teeth35 = new QPixmap(imagePath);
    emit sig_35(teeth35);   // 치아 35번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_36_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/36.png"));
    QPixmap* teeth36 = new QPixmap(imagePath);
    emit sig_36(teeth36);   // 치아 36번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_37_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomRight/37.png"));
    QPixmap* teeth37 = new QPixmap(imagePath);
    emit sig_37(teeth37);   // 치아 37번 이미지를 시그널로 전달
    close();
}

/* Bottom Left images - 치아 41번 ~ 47번 */
void TeethForm::on_toolButton_41_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/41.png"));
    QPixmap* teeth41 = new QPixmap(imagePath);
    emit sig_41(teeth41);   // 치아 41번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_42_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/42.png"));
    QPixmap* teeth42 = new QPixmap(imagePath);
    emit sig_42(teeth42);   // 치아 42번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_43_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/43.png"));
    QPixmap* teeth43 = new QPixmap(imagePath);
    emit sig_43(teeth43);   // 치아 43번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_44_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/44.png"));
    QPixmap* teeth44 = new QPixmap(imagePath);
    emit sig_44(teeth44);   // 치아 44번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_45_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/45.png"));
    QPixmap* teeth45 = new QPixmap(imagePath);
    emit sig_45(teeth45);   // 치아 45번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_46_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/46.png"));
    QPixmap* teeth46 = new QPixmap(imagePath);
    emit sig_46(teeth46);   // 치아 46번 이미지를 시그널로 전달
    close();
}

void TeethForm::on_toolButton_47_clicked()
{
    QString imagePath(tr(":/teeth/teeth/bottomLeft/47.png"));
    QPixmap* teeth47 = new QPixmap(imagePath);
    emit sig_47(teeth47);   // 치아 47번 이미지를 시그널로 전달
    close();
}


