#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QGraphicsItem>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
public slots:
    void rotate90();
private:
    QGraphicsView* view;
    QGraphicsScene* scene;

    QToolBar* editToolBar;
    QAction* rotateAction;

    void createSceneAndView();
    void createToolBarAndAction();
    void createGraphicsItems();

};

#endif // MAINWINDOW_H
