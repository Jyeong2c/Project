#ifndef BLENDING_H
#define BLENDING_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QFile>

using namespace std;
using namespace cv;

class QPixmap;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QReszieEvent;
class QListWidgetItem;

namespace Ui {
class Blending;
}

class Blending : public QDialog
{
    Q_OBJECT
public:
    explicit Blending(QWidget *parent = nullptr);
    ~Blending();

    void loadImages();
    void uiSetting();
    void setImage1(const QString& _image1);
    void setImage2(const QString& _image2);

public slots:
    void onBlending(int);

protected:
    void resizeEvent(QResizeEvent *) override;

private slots:
    void on_imageListWidget_clicked(const QModelIndex &index);

private:
    Ui::Blending *ui;
    /*원본 이미지 1, 2를 선정후 */
    Mat image1, image2;
    Mat reImage1, reImage2;

    /*GraphicsView 에 이미지를 띄울 Scene*/
    QGraphicsScene *sceneImage;

    QImage* image;

    //받는 이미지를 count 할 수 있는 변수
    int imageFlag = 0;
};

#endif // BLENDING_H
