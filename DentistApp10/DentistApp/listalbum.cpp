//#include "listalbum.h"

//#include <QSplitter>
//#include <QListWidget>
//#include <QDir>
//#include <QFileInfo>


//Listalbum::Listalbum(QWidget *parent)
//    : QSplitter(parent)
//{
//    listWidget = new QListWidget(this);
//    listWidget->setIconSize(QSize(130, 80));
//    listWidget->setResizeMode(QListWidget::Adjust);
//    listWidget->setFlow(QListWidget::LeftToRight);
//    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    setOrientation(Qt::Vertical);
//    addWidget(listWidget);

//    loadImages();
//}

//void Listalbum::loadImages()
//{
//    QDir dir(".");
//    QStringList filters;
//    filters << "*.png" << "*.jpg" << "*.bmp";
//    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
//    //graphicsScene->clear();

//    listWidget->clear();
//    for(int i=0; i<fileInfoList.count(); i++){
//        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, listWidget);
//        item->setStatusTip(fileInfoList.at(i).fileName());
//        listWidget->addItem(item);
//    };
//}
