#include <QCoreApplication>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void onBlending(int value, void* userdata)
{
    Mat image1 = imread("./lena.bmp", IMREAD_COLOR);
    Mat image2 = imread("./pepper.bmp", IMREAD_COLOR);

    CV_Assert(!(image1.empty() || image2.empty()));

    Mat image3;
    double alpha = (double)value / 100;
    addWeighted(image1, 1 - alpha, image2, alpha, 0, image3);
    imshow("result", image3);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    namedWindow("result", WINDOW_AUTOSIZE);
    /*0부터 100까지의 범위를 잡는 트랙바를 onBlending에 적용*/
    createTrackbar("blending", "result", 0, 100, onBlending);

    waitKey(0);

    return a.exec();
}
