#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <QColorDialog>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QPainter>
#include <QMessageBox>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsView *view = new QGraphicsView(this);

    scene = new Scene(this);

    view->setScene(scene);
    ui->verticalLayout->addWidget(view);

    /*길이 측정 좌표 signal과 좌표값을 받는 슬롯*/
    connect(scene, &Scene::sendFirstOrtho, this, &MainWindow::receiveFirstOrtho);
    connect(scene, &Scene::sendLastOrtho, this, &MainWindow::receiveLastOrtho);

    /*각도 측정 결과 signal과 결과값을 받는 슬롯*/
    connect(scene, &Scene::sendMeasureAngle, this, &MainWindow::receiveAngleMeasure);

//    connect(scene, &Scene::itemSelected, this, &MainWindow::itemSelected);
//    connect(scene, &Scene::textInserted, this, &MainWindow::textInserted);
//    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged,
//            this, &MainWindow::currentFontChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionImageLoad_triggered()
{
    QFileDialog dlg;
    QString filePath = dlg.getOpenFileName(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");

    QImage *imgSize = new QImage(filePath);
    QPixmap buf = QPixmap::fromImage(*imgSize);
    buf = buf.scaled(imgSize->width(), imgSize->height());

    //scene = new Scene;
    scene->addPixmap(buf);
    scene->update();

}

void MainWindow::on_actionBrush_triggered()
{
    scene->setCurrentShape(Scene::Path);
}


void MainWindow::on_actionRect_triggered()
{
    scene->setCurrentShape(Scene::Rect);
}

void MainWindow::on_actionEllipse_triggered()
{
    scene->setCurrentShape(Scene::Ellipse);
}


void MainWindow::on_actionLine_triggered()
{
    scene->setCurrentShape(Scene::Line);
}


void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()){
        scene->setCurrentColor(color);
    }
}


void MainWindow::on_actionImplantImage_triggered()
{
    scene->setCurrentShape(Scene::Image);
}


void MainWindow::on_actionClear_triggered()
{
    scene->clear();
}


void MainWindow::on_actionLength_triggered()
{
    scene->setCurrentShape(Scene::Length);
    qDebug() << "Action clicked Length!";
}


void MainWindow::on_actionAngle_triggered()
{
    scene->setCurrentShape(Scene::Angle);
    qDebug() << "Action clicked Angle!";
}


void MainWindow::on_actionText_triggered()
{
    scene->setCurrentShape(Scene::Text);
    qDebug() << "Action clicked Text!";
}

/*길이 측정 좌표의 처음 위치에서 마지막 위치까지 잡히는 계산*/

void MainWindow::receiveFirstOrtho(int _x, int _y)
{
    /*길이 측정 액션을 선택한 경우에만 텍스트를 출력*/
    if(scene->getCurrentShape() == Scene::Length)
    {
        //        QGraphicsTextItem *text = new QGraphicsTextItem();
        //        text = scene->addText(QString("%1, %2").arg(_x).arg(_y));
        //        text->setPos(_x, _y);
        setFirstX(_x);
        setFirstY(_y);
    }
}

void MainWindow::setFirstX(int &x)
{
    firstX = x;
}

void MainWindow::setFirstY(int &y)
{
    firstY = y;
}

/*텍스트 설정 관련 슬롯 함수*/
//void MainWindow::textInserted(QGraphicsTextItem *)
//{
//    scene->setMode(Scene::Shape());
//}
//void MainWindow::currentFontChanged(const QFont &font)
//{
//    handleFontChange();
//}
//void MainWindow::fontSizeChangeed(const QString &size)
//{
//    handleFontChange();
//}
//void MainWindow::itemSelected(QGraphicsItem *item)
//{
//    TextItem *textItem =
//            qgraphicsitem_cast<TextItem *>(item);

//    QFont font = textItem->font();
//    ui->fontComboBox->setCurrentFont(font);
//    ui->comboBox->setEditText(QString().setNum(font.pointSize()));
//}

//void MainWindow::textButtonTriggered()
//{
//    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
//}

//void MainWindow::handleFontChange()
//{
//    QFont font = ui->fontComboBox->currentFont();
//    font.setPointSize(ui->comboBox->currentText().toInt());
//    scene->setFont(font);
//}

/*길이 측정 첫번째 좌표와 결과값을 출력하는 슬롯 함수 구현부*/
void MainWindow::receiveLastOrtho(int _x, int _y)
{
    /*길이 측정 액션을 선택한 경우에만 텍스트를 출력*/
    if(scene->getCurrentShape() == Scene::Length)
    {
        qDebug() << "firstX : " << firstX << ", firstY : " << firstY;
        qDebug() << "lastX : " << _x << ", lastY : " << _y;

        /*길이측정 공식 : z = sqrt( (x2 - x1)^2 + (y2 - y1)^2 )*/
        int z = qSqrt(qPow((_x - firstX), 2) + qPow((_y - firstY), 2));


        QMessageBox::information(this, "length measure",
                                 QString("Length : %1").arg(z));


        /*하다못해 이문제를 해결하지 못하면 메세지 박스와 라인 에디트로 길이를 출력해야함*/
//        QGraphicsTextItem* ti = new QGraphicsTextItem(QString("%1").arg(z));
//        ti->setTextInteractionFlags(Qt::TextEditorInteraction);
//        ti->setPos(_x, _y);
//        scene->addItem(ti);
        //text->deleteLater();
    }
}

/*각도 측정 결과값을 출력하는 슬롯 함수 구현부*/
void MainWindow::receiveAngleMeasure(double angle)
{
    /*길이 측정 슬롯과 마찬 가지로 Scene 상에 각도 결과를 나타낼 수 없으면 라인에디트나 메세지박스로 출력*/
    QMessageBox::information(this, "Angle Result",
                             QString("Angle : %1").arg(angle));
}



