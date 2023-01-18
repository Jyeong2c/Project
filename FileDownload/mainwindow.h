#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FileDownloader;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();

private:
    FileDownloader *m_pImgCtrl;
};
#endif // MAINWINDOW_H
