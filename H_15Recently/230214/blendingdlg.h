#ifndef BLENDINGDLG_H
#define BLENDINGDLG_H

#include <QDialog>

/*영상처리를 위한 opencv 헤더 선언*/
#include <opencv2/opencv.hpp>
#include <QFile>

using namespace std;
using namespace cv;

/*Qt에서 제공하는 QClass 집합*/
class QPixmap;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QReszieEvent;
class QListWidgetItem;


namespace Ui {
class BlendingDlg;
}

class BlendingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BlendingDlg(QWidget *parent = nullptr);
    ~BlendingDlg();

    void loadImages();      // listWidget의 이미지를 출력하기 위한 로드 함수
    void uiSetting();       // blending dialog 내부 ui 설정, 비율 설정 함수

    /*선택된 이미지를 받는 setImage1,2 함수*/
    void setImage1(const QString& _image1);
    void setImage2(const QString& _image2);

    void copyPath(QString, QString);            // 파일과 폴더를 복사할 수 있는 함수


public slots:
    void onBlending(int);   // 슬라이더 위젯을 움직일 때 마다 가중치 값을 받는 함수

protected:
    void resizeEvent(QResizeEvent *) override;  // resizeEvent 함수 override

private slots:
    void on_imageListWidget_clicked(const QModelIndex &index); // listWidget을 클릭 시 이미지 호출

private:
    Ui::BlendingDlg *ui;

    /*원본 이미지 1, 2*/
    Mat image1, image2;
    /*resize된 이미지 1, 2*/
    Mat reImage1, reImage2;

    QGraphicsScene *sceneImage; // GraphicsView 에 이미지를 띄울 Scene
    QImage* image;          // 결과 이미지를 할당하기 위한 변수
    int imageFlag = 0;      // 받는 이미지를 count 할 수 있는 변수
};

#endif // BLENDINGDLG_H
