#include "mainwindow.h"

#include <QUrl>
#include "filedownloader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QUrl imageUrl("http://192.168.0.48:3000/Files/P1_14FMX1.png");
    m_pImgCtrl = new FileDownloader(imageUrl, this);

    connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadImage()
{
    QPixmap buttonImage;
    buttonImage.loadFromData(m_pImgCtrl->downloadedData());
    qDebug() << m_pImgCtrl->downloadedData();

}

