#include "teethform.h"
#include "ui_teethform.h"


#include <QLabel>

TeethForm::TeethForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeethForm)
{
    ui->setupUi(this);


}

TeethForm::~TeethForm()
{
    delete ui;
}


/* Top Left */
void TeethForm::on_toolButton_11_clicked()
{
//    QPixmap p; // load pixmap
//    // get label dimensions
//    int w = label->width();
//    int h = label->height();

//    // set a scaled pixmap to a w x h window keeping its aspect ratio
//    label->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
//    축소할 수 있으려면 다음 호출을 추가해야 합니다.label->setMinimumSize(1, 1)

//    QPixmap p;
//    int w = 10;
//    int h = 10;



    QString imagePath(tr(":/teeth/teeth/topLeft/11.png"));
    QPixmap* teeth11 = new QPixmap(imagePath);

    emit sig_11(teeth11);

    close();
}


void TeethForm::on_toolButton_12_clicked()
{
    close();
}


void TeethForm::on_toolButton_13_clicked()
{

}


void TeethForm::on_toolButton_14_clicked()
{

}


void TeethForm::on_toolButton_15_clicked()
{

}


void TeethForm::on_toolButton_16_clicked()
{

}

void TeethForm::on_toolButton_17_clicked()
{

}


/* Top Right */
void TeethForm::on_toolButton_21_clicked()
{

}


void TeethForm::on_toolButton_22_clicked()
{

}


void TeethForm::on_toolButton_23_clicked()
{

}


void TeethForm::on_toolButton_24_clicked()
{

}


void TeethForm::on_toolButton_25_clicked()
{

}


void TeethForm::on_toolButton_26_clicked()
{

}


void TeethForm::on_toolButton_27_clicked()
{

}


/* Bottom Right */
void TeethForm::on_toolButton_31_clicked()
{

}


void TeethForm::on_toolButton_32_clicked()
{

}


void TeethForm::on_toolButton_33_clicked()
{

}


void TeethForm::on_toolButton_34_clicked()
{

}


void TeethForm::on_toolButton_35_clicked()
{

}


void TeethForm::on_toolButton_36_clicked()
{

}


void TeethForm::on_toolButton_37_clicked()
{

}


/* Bottom Left */
void TeethForm::on_toolButton_41_clicked()
{

}


void TeethForm::on_toolButton_42_clicked()
{

}


void TeethForm::on_toolButton_43_clicked()
{

}


void TeethForm::on_toolButton_44_clicked()
{

}


void TeethForm::on_toolButton_45_clicked()
{

}


void TeethForm::on_toolButton_46_clicked()
{

}


void TeethForm::on_toolButton_47_clicked()
{

}




