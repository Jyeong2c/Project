#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QActionGroup>
#include <QToolBar>
#include "scene.h"
#include <QAction>

/*아래 코드 조각은 QMainWindow를 하위 분류하는 방법을 보여줍니다.
 * 선 그리기 모드는 lineAction으로 표시되고 선택 모드는 Action을 선택하여 표시됩니다.
 * QAction Group이 도입되어 주어진 시간에 lineAction 또는 selectAction 중 하나만 활성화됩니다.*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
public slots:
    void actionGroupClicked(QAction*);
    void OrthoReceive(int x, int y);
private:
    QGraphicsView* view;
    Scene* scene;

    /*MainWindow::createActions()에서 lineAction을 인스턴스화하고 Action을 선택하여 ActionGroup에 추가합니다.
     * 또한 QActions를 특정 장면 모드와 연결하기 위해 QActions에서 setData(QVariant)를 호출합니다.*/
    void createActions();
    void createConnections();
    void createToolBar();

    /*Action을 선택한 후 이 두 가지 작업의 배타성을 보장하는 QAction Group을 선택합니다*/
    QAction* lineAction;
    QAction* selectAction;
    QActionGroup *actionGroup;
    QToolBar* drawingToolBar;

    QGraphicsTextItem *text;
};

#endif // MAINWINDOW_H
