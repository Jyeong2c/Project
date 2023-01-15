#include "mainwindow.h"
#include <QtMath>
#include <QMessageBox>

MainWindow::MainWindow()
{
    scene = new Scene(this);
    scene->setSceneRect(0,0,200,200);
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    text = new QGraphicsTextItem();
    text->setDefaultTextColor(Qt::black);
    setCentralWidget(view);

    createActions();
    createConnections();
    createToolBar();
}

/*MainWindow::createActions()에서 lineAction을 인스턴스화하고 Action을 선택하여 ActionGroup에 추가합니다.
 * 또한 QActions를 특정 장면 모드와 연결하기 위해 QActions에서 setData(QVariant)를 호출합니다.*/
void MainWindow::createActions(){
    lineAction = new QAction("Draw line", this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setIcon(QIcon("D:/GitDesktop/Project/QGraphicsLineItemApp/line.png"));
    lineAction->setCheckable(true);             //체크 상태 ON

    selectAction = new QAction("Select object", this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setIcon(QIcon("D:/GitDesktop/Project/QGraphicsLineItemApp/select.png"));
    selectAction->setCheckable(true);           //체크 상태 ON

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);            //exculsive를 함으로 선택된 버튼 이외의 버튼은 비체크 상태로 전환
    actionGroup->addAction(lineAction);
    actionGroup->addAction(selectAction);
}

/*MainWindow::createConnections()에서는 작업 그룹의 신호 트리거(QAction*)를 슬롯 작업 그룹 클릭(QAction*)에 연결합니다.
 * 아래에서 볼 수 있듯이 클릭된 그룹(QAction*) 작업은 장면의 모드를 설정하는 것입니다.*/
void MainWindow::createConnections(){
    connect(actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(actionGroupClicked(QAction*)));
    connect(scene, SIGNAL(OrthoSend(int,int)), this, SLOT(OrthoReceive(int,int)));
}

void MainWindow::actionGroupClicked(QAction *action){
    scene->setMode(Scene::Mode(action->data().toInt()));
}

/*toolbar 메뉴 추가*/
void MainWindow::createToolBar(){
    drawingToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, drawingToolBar);
    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(lineAction);
}

int num = 0;
int x_1 = 0;
int y_1 = 0;
int x_2 = 0;
int y_2 = 0;
void MainWindow::OrthoReceive(int _x, int _y){
    update();
    if(!selectAction->isChecked()){

//        text = new QGraphicsTextItem();
//        text = scene->addText(QString("%1, %2").arg(_x).arg(_y));
//        text->setPos(_x, _y);
        qDebug() << "OrthoReceive : " << _x << ", " << _y;
        qDebug() << "check : " << num;
        if(num == 0){
            x_1 = _x;
            y_1 = _y;
        }
        if(num == 1){
            x_2 = _x;
            y_2 = _y;
            qDebug() << "x1 = " << x_1 <<
                        "y1 = " << y_1 <<
                        "x2 = " << x_2 <<
                        "y2 = " << y_2;

            //삼각함수 기법을 활용한 line의 길이 측정
            int z = qSqrt(qPow((x_2 - x_1), 2) + qPow((y_2 - y_1), 2));
            qDebug() << "z : " << z;
            //text = scene->addText(QString("%1").arg(z));
            //text->setPos(x_2, y_2);
            QMessageBox::information(this,"length",
                                    QString("length info : %1").arg(z / 20));
            //사진의 축적 비율 정보만 알면 실제 길이를 측정 할 수 있을 것 같음
        }

        num++;
        if(num > 1){
            selectAction->setCheckable(true);
            selectAction->setChecked(true);
            lineAction->setChecked(false);
            scene->setMode(Scene::SelectObject);
            //text->setEnabled(false);
            //delete text;
            num = 0;
        }
    }else{
        return;
    }
}
