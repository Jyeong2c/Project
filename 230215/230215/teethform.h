#ifndef TEETHFORM_H
#define TEETHFORM_H


#include <QWidget>

#include <QLabel>


namespace Ui {
class TeethForm;
}

class TeethForm : public QWidget
{
    Q_OBJECT

public:
    explicit TeethForm(QWidget *parent = nullptr);
    ~TeethForm();



private slots:
    void on_toolButton_11_clicked();
    void on_toolButton_12_clicked();
    void on_toolButton_13_clicked();
    void on_toolButton_14_clicked();
    void on_toolButton_15_clicked();
    void on_toolButton_16_clicked();
    void on_toolButton_17_clicked();

    void on_toolButton_21_clicked();
    void on_toolButton_22_clicked();
    void on_toolButton_23_clicked();
    void on_toolButton_24_clicked();
    void on_toolButton_25_clicked();
    void on_toolButton_26_clicked();
    void on_toolButton_27_clicked();

    void on_toolButton_31_clicked();
    void on_toolButton_32_clicked();
    void on_toolButton_33_clicked();
    void on_toolButton_34_clicked();
    void on_toolButton_35_clicked();
    void on_toolButton_36_clicked();
    void on_toolButton_37_clicked();

    void on_toolButton_41_clicked();
    void on_toolButton_42_clicked();
    void on_toolButton_43_clicked();
    void on_toolButton_44_clicked();
    void on_toolButton_45_clicked();
    void on_toolButton_46_clicked();
    void on_toolButton_47_clicked();

private:
    Ui::TeethForm *ui;
//    QList<QString> m_toothGroups;

signals:
    /* Top Left Teeth */
//    void sig_11(QString);

    void sig_11(QPixmap*);
    void sig_12(QPixmap*);
    void sig_13(QPixmap*);
    void sig_14(QPixmap*);
    void sig_15(QPixmap*);
    void sig_16(QPixmap*);
    void sig_17(QPixmap*);

    /* Top Right Teeth */
    void sig_21(QPixmap*);
    void sig_22(QPixmap*);
    void sig_23(QPixmap*);
    void sig_24(QPixmap*);
    void sig_25(QPixmap*);
    void sig_26(QPixmap*);
    void sig_27(QPixmap*);

    /* Bottom Right Teeth */
    void sig_31(QPixmap*);
    void sig_32(QPixmap*);
    void sig_33(QPixmap*);
    void sig_34(QPixmap*);
    void sig_35(QPixmap*);
    void sig_36(QPixmap*);
    void sig_37(QPixmap*);

    /* Bottom Left Teeth */
    void sig_41(QPixmap*);
    void sig_42(QPixmap*);
    void sig_43(QPixmap*);
    void sig_44(QPixmap*);
    void sig_45(QPixmap*);
    void sig_46(QPixmap*);
    void sig_47(QPixmap*);
};

#endif // TEETHFORM_H
