#include "qdownloader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDownloader w;
    /*만약 파일이 다운로드 되지 않으면 main문 수정*/
    w.setFile("http://192.168.0.48:3000/Files/panorama1.png");
    return a.exec();
}
