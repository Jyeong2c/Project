#include "newwindow.h"
#include "layout.h"

#include <QGraphicsView>
#include <QGraphicsScene>

NewWindow::NewWindow(QWidget *parent)
    : QWidget{parent}
{
//    customLyaout->scene
    customLyaout->grid->setScene(customLyaout->scene);
}
