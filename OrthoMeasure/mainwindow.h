#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsView;
class Scene;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void actionGroupClicked(QAction*);

private:
    Ui::MainWindow *ui;
    QGraphicsView* view;
    Scene* scene;

    void createActions();
    void createConnections();
    void createToolBar();

    QAction* lineAction;
    QAction* selectAction;
    QActionGroup *actionGroup;
    QToolBar* drawingToolBar;
};
#endif // MAINWINDOW_H
