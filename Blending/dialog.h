#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QFile>

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

public slots:
    void onBlending(int);

private:
    Ui::Dialog *ui;
    Mat image1, image2;
    Mat reImage1, reImage2;
};
#endif // DIALOG_H
