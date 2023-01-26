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

namespace Ui {
class Blending;
}

class Blending : public QDialog
{
    Q_OBJECT

public:
    explicit Blending(QWidget *parent = nullptr);
    ~Blending();

public slots:
    void onBlending(int);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    Ui::Blending *ui;
    /*원본 이미지 1, 2를 선정후 */
    Mat image1, image2;
    Mat reImage1, reImage2;

    /*GraphicsView 에 이미지를 띄울 Scene*/
    QGraphicsScene *sceneImage;

    QImage* image;

    /*바운딩 렉트를 위한 Hash 할당*/
    //QHash<QGraphicsPixmapItem*, QString> itemHash;
};

#endif // BLENDING_H
