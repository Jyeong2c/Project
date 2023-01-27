#include "brightness.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Brightness::Brightness(QWidget *parent)
    : QWidget{parent}
{
}

void Brightness::contrast()
{
    Mat image = imread("../image/lena.bmp",0);
    CV_Assert(image.data);

    Scalar avg = mean(image) /2.0;
    Mat dst1 = image * 0.5;
    Mat dst2 = image * 2.0;
    Mat dst3 = image * 0.5 + avg[0];
    Mat dst4 = image * 2.0 - avg[0];

    imshow("image",image);
    imshow("dst1-대비감소",dst1), imshow("dst2-대비증가",dst2);
    imshow("dst3-평균이용 대비감소",dst3) , imshow("dst4-평균이용 대비증가",dst4);
    waitKey();

}
void Brightness::bright()
{
    Mat image = imread("../image/lena.bmp",IMREAD_GRAYSCALE);
    CV_Assert(!image.empty());

    Mat dst1 = image +100;
    Mat dst2 = image -100;
    Mat dst3 = 255 -image;

    Mat dst4(image.size(),image.type());
    Mat dst5(image.size(),image.type());
    for(int i =0;i<image.rows; i++){
        for(int j =0; j<image.cols; j++)
        {
            dst4.at<uchar>(i,j) =image.at<uchar>(i,j) +100;
            dst5.at<uchar>(i,j) =255 - image.at<uchar>(i,j);
        }
    }
    imshow("원 영상 ", image), imshow("dst1 - 밝기",dst1);
    imshow("dst2-어둡게 ", dst2), imshow("dst3 - 반전",dst3);
    imshow("dst4 - 밝게 ", dst4), imshow("dst1 - 반전",dst5);
    waitKey();
}
