#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
//#include "fmx.h"
#include "maxlayout.h"
#include "keyfeaturesform.h"
#include "teethform.h"
//#include "menuitem.h"
#include "view.h"
#include "scene.h"
#include "blendingdlg.h"
#include "patientmodel.h"
#include "patientview.h"
#include "uploaddlg.h"
#include "deleteimage.h"

#include <opencv2/opencv.hpp>
using namespace cv;

#include <QtTest/QtTest>
#include <QTest>
#include <QSplitter>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QPalette>
#include <QMdiSubWindow>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPoint>
#include <QRectF>
#include <QHash>
#include <QStackedWidget>
#include <QImage>
#include <QMenu>
#include <QGraphicsItem>
#include <QColorDialog>         // EUN JI

/*JAE HY*/
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QTableWidgetItem>
#include <QImageReader>

//Json에 Parsing과 관련된 헤더
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/* JAE YEONG*/
#include <QFileDialog>
#include <QMessageBox>
/* 환자 정보 DB */
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>


#define sheetWhite "background:rgb(255, 255, 255)"
#define sheetNavy "background:rgb(32, 56, 100)"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)/*, newScene(nullptr)*/
{    
    ui->setupUi(this);

    /* WindowTitle 지정 */
    stackWidget = new QStackedWidget(this);
    //여기서 글꼴과 색상을 변경
    //stackWidget->setWindowTitle(tr("2 X 2 Grid"));
    //QString("<span style=\" color:#ff0000;\">%1</span>").arg(">>>");
    //QString titleName = QString("<span style=\" color:#ff0000;\">%1</span>").arg(tr("2 x 2 Grid"));
    QFont f("Bold", 50);
    QString redPart = "2 x 2 Grid";
    stackWidget->setWindowTitle(redPart);
    stackWidget->setStyleSheet("font-weight : bold; color : red");
    connect(stackWidget, SIGNAL(destroyed()), stackWidget, SLOT(deleteLater()));


    teeth = new TeethForm();
    connect(teeth, SIGNAL(destroyed()), teeth, SLOT(deleteLater()));

    customLayout = new Layout(this);                                                // 레이아웃 화면
    connect(customLayout, SIGNAL(destroyed()), customLayout, SLOT(deleteLater()));

    keyFeatures = new KeyFeaturesForm(this);                                              // 주요 기능 클래스
    connect(keyFeatures, SIGNAL(destroyed()), keyFeatures, SLOT(deleteLater()));


    myMaxlayout = new Maxlayout(this);                                              // 최대 창
    connect(myMaxlayout, SIGNAL(destroyed()), myMaxlayout, SLOT(deleteLater()));

     //재현

    /*네트워크 접근을 위한 manager 변수 new 할당 eventLoop로 서버 동기화*/
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));


    stackWidget->insertWidget(0, customLayout);
    stackWidget->insertWidget(1, myMaxlayout);
    stackWidget->setCurrentIndex(0);
    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(stackWidget);
    stackWidget->showMaximized();
//    ui->mdiArea->addSubWindow(fmx);
    //cw->setStyleSheet("background:rgb(32, 56, 100)");
    cw->setStyleSheet("background:rgb(0, 0, 0)");
    ui->mdiArea->setActiveSubWindow(cw);


    QVBoxLayout *KeyLayout = new QVBoxLayout();
    KeyLayout->addWidget(keyFeatures);
    ui->frame->setLayout(KeyLayout);


    styleColor();                                   // ui 색상 조절 및 크기 조절 함수
    //loadImages();                                   // listWidget에 이미지 로드

    ui->listWidget->setIconSize(QSize(130, 80));
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    /* 시그널 슬롯은 위치가 중요 동적할당(new)보다 밑에 있을 것 */
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));
    connect(customLayout, SIGNAL(sig_widgetbyDClick()), SLOT(slot_doubleSize()));           // 최대 창 size 맞춰주는 슬롯
    connect(myMaxlayout->viewQuit, SIGNAL(clicked()), SLOT(previousScreen()));
    connect(myMaxlayout, SIGNAL(sig_clicked(QPointF)), customLayout, SLOT(showMenu(QPointF)));


    connect(keyFeatures, SIGNAL(sig_keyCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)),
            customLayout, SLOT(slot_keyCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)));
    connect(keyFeatures, SIGNAL(sig_logCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)),
            customLayout, SLOT(slot_logCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)));
    connect(keyFeatures, SIGNAL(sig_save(bool)), customLayout, SLOT(slot_save(bool)));

    //    keyFeatures->keepCheck();

    /* 유나 기능 */
    connect(customLayout->zoomInAction, SIGNAL(triggered()), SLOT(on_zoomInToolButton_clicked()));
    connect(customLayout->zoomOutAction, SIGNAL(triggered()), SLOT(on_zoomOutToolButton_clicked()));
    connect(customLayout->leftRotateAction, SIGNAL(triggered()), SLOT(on_leftRotateToolButton_clicked()));
    connect(customLayout->rightRotateAction, SIGNAL(triggered()), SLOT(on_rightRotateToolButton_clicked()));
    connect(customLayout->sourceSizeAction, SIGNAL(triggered()), SLOT(on_sourceSizePushButton_clicked()));
    connect(customLayout->layoutImageClearAction, SIGNAL(triggered()), SLOT(on_layoutClearPushButton_clicked()));
    connect(customLayout->implantAction, SIGNAL(triggered()), SLOT(on_implantToolButton_clicked()));
    connect(customLayout->implantClearAction, SIGNAL(triggered()), SLOT(on_implantClearToolButton_clicked()));

    /* 은지 기능 */
    connect(customLayout->brushAction, SIGNAL(triggered()), SLOT(on_brushToolButton_clicked()));
    connect(customLayout->rectangleAction, SIGNAL(triggered()), SLOT(on_rectangleToolButton_clicked()));
    connect(customLayout->triangleAction, SIGNAL(triggered()), SLOT(on_triangleToolButton_clicked()));
    connect(customLayout->ellipseAction, SIGNAL(triggered()), SLOT(on_circleToolButton_clicked()));
    connect(customLayout->cursorAction, SIGNAL(triggered()), SLOT(on_cursorToolButton_clicked()));
    connect(customLayout->drawClearAction, SIGNAL(triggered()), SLOT(on_drawClearPushButton_clicked()));
    connect(customLayout->arrowAction, SIGNAL(triggered()), SLOT(on_arrowToolButton_clicked()));
    connect(customLayout->lineAction, SIGNAL(triggered()), SLOT(on_lineToolButton_clicked()));

    /* 재영 기능 */
    connect(customLayout->brightnessAction, SIGNAL(triggered()), SLOT(brightnessButton()));
    connect(customLayout->darknessAction, SIGNAL(triggered()), SLOT(darknessButton()));
    connect(customLayout->sharpenAction, SIGNAL(triggered()), SLOT(on_sharpenToolButton_clicked()));
    connect(customLayout->contrastAction, SIGNAL(triggered()), SLOT(contrastButton()));
    connect(customLayout->inversionAction, SIGNAL(triggered()), SLOT(on_inversionToolButton_clicked()));
    connect(customLayout->verticalFlipAction, SIGNAL(triggered()), SLOT(on_verticalFlipToolButton_clicked()));
    connect(customLayout->horizontalFlipAction, SIGNAL(triggered()), SLOT(on_horizontalFlipToolButton_clicked()));
    connect(customLayout->imageProcessingClearAction, SIGNAL(triggered()), SLOT(on_processingClearPushButton_clicked()));

    /* 재현 기능 */
    connect(customLayout->lengthMeasurementAction, SIGNAL(triggered()), SLOT(on_rulerToolButton_clicked()));
    connect(customLayout->angleMeasurementAction, SIGNAL(triggered()), SLOT(on_protractorToolButton_clicked()));


    // 배열로 만든 이미지 경로 시그널 슬롯
//    connect(teeth, SIGNAL(sig_11(QString)), customLayout->scene1, SLOT(slot_11(QString)));


    /* Top Left Teeth - scene1 */
    connect(teeth, SIGNAL(sig_11(QPixmap*)), customLayout->scene1, SLOT(slot_11(QPixmap*)));
    connect(teeth, SIGNAL(sig_12(QPixmap*)), customLayout->scene1, SLOT(slot_12(QPixmap*)));
    connect(teeth, SIGNAL(sig_13(QPixmap*)), customLayout->scene1, SLOT(slot_13(QPixmap*)));
    connect(teeth, SIGNAL(sig_14(QPixmap*)), customLayout->scene1, SLOT(slot_14(QPixmap*)));
    connect(teeth, SIGNAL(sig_15(QPixmap*)), customLayout->scene1, SLOT(slot_15(QPixmap*)));
    connect(teeth, SIGNAL(sig_16(QPixmap*)), customLayout->scene1, SLOT(slot_16(QPixmap*)));
    connect(teeth, SIGNAL(sig_17(QPixmap*)), customLayout->scene1, SLOT(slot_17(QPixmap*)));

    /* Top Right Teeth - scene1 */
    connect(teeth, SIGNAL(sig_21(QPixmap*)), customLayout->scene1, SLOT(slot_21(QPixmap*)));
    connect(teeth, SIGNAL(sig_22(QPixmap*)), customLayout->scene1, SLOT(slot_22(QPixmap*)));
    connect(teeth, SIGNAL(sig_23(QPixmap*)), customLayout->scene1, SLOT(slot_23(QPixmap*)));
    connect(teeth, SIGNAL(sig_24(QPixmap*)), customLayout->scene1, SLOT(slot_24(QPixmap*)));
    connect(teeth, SIGNAL(sig_25(QPixmap*)), customLayout->scene1, SLOT(slot_25(QPixmap*)));
    connect(teeth, SIGNAL(sig_26(QPixmap*)), customLayout->scene1, SLOT(slot_26(QPixmap*)));
    connect(teeth, SIGNAL(sig_27(QPixmap*)), customLayout->scene1, SLOT(slot_27(QPixmap*)));

    /* Bottom Right Teeth - scene1 */
    connect(teeth, SIGNAL(sig_31(QPixmap*)), customLayout->scene1, SLOT(slot_31(QPixmap*)));
    connect(teeth, SIGNAL(sig_32(QPixmap*)), customLayout->scene1, SLOT(slot_32(QPixmap*)));
    connect(teeth, SIGNAL(sig_33(QPixmap*)), customLayout->scene1, SLOT(slot_33(QPixmap*)));
    connect(teeth, SIGNAL(sig_34(QPixmap*)), customLayout->scene1, SLOT(slot_34(QPixmap*)));
    connect(teeth, SIGNAL(sig_35(QPixmap*)), customLayout->scene1, SLOT(slot_35(QPixmap*)));
    connect(teeth, SIGNAL(sig_36(QPixmap*)), customLayout->scene1, SLOT(slot_36(QPixmap*)));
    connect(teeth, SIGNAL(sig_37(QPixmap*)), customLayout->scene1, SLOT(slot_37(QPixmap*)));

    /* Bottom Left Teeth - scene1 */
    connect(teeth, SIGNAL(sig_41(QPixmap*)), customLayout->scene1, SLOT(slot_41(QPixmap*)));
    connect(teeth, SIGNAL(sig_42(QPixmap*)), customLayout->scene1, SLOT(slot_42(QPixmap*)));
    connect(teeth, SIGNAL(sig_43(QPixmap*)), customLayout->scene1, SLOT(slot_43(QPixmap*)));
    connect(teeth, SIGNAL(sig_44(QPixmap*)), customLayout->scene1, SLOT(slot_44(QPixmap*)));
    connect(teeth, SIGNAL(sig_45(QPixmap*)), customLayout->scene1, SLOT(slot_45(QPixmap*)));
    connect(teeth, SIGNAL(sig_46(QPixmap*)), customLayout->scene1, SLOT(slot_46(QPixmap*)));
    connect(teeth, SIGNAL(sig_47(QPixmap*)), customLayout->scene1, SLOT(slot_47(QPixmap*)));

    /* Top Left Teeth - scene2 */
    connect(teeth, SIGNAL(sig_11(QPixmap*)), customLayout->scene2, SLOT(slot_11(QPixmap*)));
    connect(teeth, SIGNAL(sig_12(QPixmap*)), customLayout->scene2, SLOT(slot_12(QPixmap*)));
    connect(teeth, SIGNAL(sig_13(QPixmap*)), customLayout->scene2, SLOT(slot_13(QPixmap*)));
    connect(teeth, SIGNAL(sig_14(QPixmap*)), customLayout->scene2, SLOT(slot_14(QPixmap*)));
    connect(teeth, SIGNAL(sig_15(QPixmap*)), customLayout->scene2, SLOT(slot_15(QPixmap*)));
    connect(teeth, SIGNAL(sig_16(QPixmap*)), customLayout->scene2, SLOT(slot_16(QPixmap*)));
    connect(teeth, SIGNAL(sig_17(QPixmap*)), customLayout->scene2, SLOT(slot_17(QPixmap*)));

    /* Top Right Teeth - scene2 */
    connect(teeth, SIGNAL(sig_21(QPixmap*)), customLayout->scene2, SLOT(slot_21(QPixmap*)));
    connect(teeth, SIGNAL(sig_22(QPixmap*)), customLayout->scene2, SLOT(slot_22(QPixmap*)));
    connect(teeth, SIGNAL(sig_23(QPixmap*)), customLayout->scene2, SLOT(slot_23(QPixmap*)));
    connect(teeth, SIGNAL(sig_24(QPixmap*)), customLayout->scene2, SLOT(slot_24(QPixmap*)));
    connect(teeth, SIGNAL(sig_25(QPixmap*)), customLayout->scene2, SLOT(slot_25(QPixmap*)));
    connect(teeth, SIGNAL(sig_26(QPixmap*)), customLayout->scene2, SLOT(slot_26(QPixmap*)));
    connect(teeth, SIGNAL(sig_27(QPixmap*)), customLayout->scene2, SLOT(slot_27(QPixmap*)));

    /* Bottom Right Teeth - scene2 */
    connect(teeth, SIGNAL(sig_31(QPixmap*)), customLayout->scene2, SLOT(slot_31(QPixmap*)));
    connect(teeth, SIGNAL(sig_32(QPixmap*)), customLayout->scene2, SLOT(slot_32(QPixmap*)));
    connect(teeth, SIGNAL(sig_33(QPixmap*)), customLayout->scene2, SLOT(slot_33(QPixmap*)));
    connect(teeth, SIGNAL(sig_34(QPixmap*)), customLayout->scene2, SLOT(slot_34(QPixmap*)));
    connect(teeth, SIGNAL(sig_35(QPixmap*)), customLayout->scene2, SLOT(slot_35(QPixmap*)));
    connect(teeth, SIGNAL(sig_36(QPixmap*)), customLayout->scene2, SLOT(slot_36(QPixmap*)));
    connect(teeth, SIGNAL(sig_37(QPixmap*)), customLayout->scene2, SLOT(slot_37(QPixmap*)));

    /* Bottom Left Teeth - scene2 */
    connect(teeth, SIGNAL(sig_41(QPixmap*)), customLayout->scene2, SLOT(slot_41(QPixmap*)));
    connect(teeth, SIGNAL(sig_42(QPixmap*)), customLayout->scene2, SLOT(slot_42(QPixmap*)));
    connect(teeth, SIGNAL(sig_43(QPixmap*)), customLayout->scene2, SLOT(slot_43(QPixmap*)));
    connect(teeth, SIGNAL(sig_44(QPixmap*)), customLayout->scene2, SLOT(slot_44(QPixmap*)));
    connect(teeth, SIGNAL(sig_45(QPixmap*)), customLayout->scene2, SLOT(slot_45(QPixmap*)));
    connect(teeth, SIGNAL(sig_46(QPixmap*)), customLayout->scene2, SLOT(slot_46(QPixmap*)));
    connect(teeth, SIGNAL(sig_47(QPixmap*)), customLayout->scene2, SLOT(slot_47(QPixmap*)));

    /* Top Left Teeth - scene3 */
    connect(teeth, SIGNAL(sig_11(QPixmap*)), customLayout->scene3, SLOT(slot_11(QPixmap*)));
    connect(teeth, SIGNAL(sig_12(QPixmap*)), customLayout->scene3, SLOT(slot_12(QPixmap*)));
    connect(teeth, SIGNAL(sig_13(QPixmap*)), customLayout->scene3, SLOT(slot_13(QPixmap*)));
    connect(teeth, SIGNAL(sig_14(QPixmap*)), customLayout->scene3, SLOT(slot_14(QPixmap*)));
    connect(teeth, SIGNAL(sig_15(QPixmap*)), customLayout->scene3, SLOT(slot_15(QPixmap*)));
    connect(teeth, SIGNAL(sig_16(QPixmap*)), customLayout->scene3, SLOT(slot_16(QPixmap*)));
    connect(teeth, SIGNAL(sig_17(QPixmap*)), customLayout->scene3, SLOT(slot_17(QPixmap*)));

    /* Top Right Teeth - scene3 */
    connect(teeth, SIGNAL(sig_21(QPixmap*)), customLayout->scene3, SLOT(slot_21(QPixmap*)));
    connect(teeth, SIGNAL(sig_22(QPixmap*)), customLayout->scene3, SLOT(slot_22(QPixmap*)));
    connect(teeth, SIGNAL(sig_23(QPixmap*)), customLayout->scene3, SLOT(slot_23(QPixmap*)));
    connect(teeth, SIGNAL(sig_24(QPixmap*)), customLayout->scene3, SLOT(slot_24(QPixmap*)));
    connect(teeth, SIGNAL(sig_25(QPixmap*)), customLayout->scene3, SLOT(slot_25(QPixmap*)));
    connect(teeth, SIGNAL(sig_26(QPixmap*)), customLayout->scene3, SLOT(slot_26(QPixmap*)));
    connect(teeth, SIGNAL(sig_27(QPixmap*)), customLayout->scene3, SLOT(slot_27(QPixmap*)));

    /* Bottom Right Teeth - scene3 */
    connect(teeth, SIGNAL(sig_31(QPixmap*)), customLayout->scene3, SLOT(slot_31(QPixmap*)));
    connect(teeth, SIGNAL(sig_32(QPixmap*)), customLayout->scene3, SLOT(slot_32(QPixmap*)));
    connect(teeth, SIGNAL(sig_33(QPixmap*)), customLayout->scene3, SLOT(slot_33(QPixmap*)));
    connect(teeth, SIGNAL(sig_34(QPixmap*)), customLayout->scene3, SLOT(slot_34(QPixmap*)));
    connect(teeth, SIGNAL(sig_35(QPixmap*)), customLayout->scene3, SLOT(slot_35(QPixmap*)));
    connect(teeth, SIGNAL(sig_36(QPixmap*)), customLayout->scene3, SLOT(slot_36(QPixmap*)));
    connect(teeth, SIGNAL(sig_37(QPixmap*)), customLayout->scene3, SLOT(slot_37(QPixmap*)));

    /* Bottom Left Teeth - scene3 */
    connect(teeth, SIGNAL(sig_41(QPixmap*)), customLayout->scene3, SLOT(slot_41(QPixmap*)));
    connect(teeth, SIGNAL(sig_42(QPixmap*)), customLayout->scene3, SLOT(slot_42(QPixmap*)));
    connect(teeth, SIGNAL(sig_43(QPixmap*)), customLayout->scene3, SLOT(slot_43(QPixmap*)));
    connect(teeth, SIGNAL(sig_44(QPixmap*)), customLayout->scene3, SLOT(slot_44(QPixmap*)));
    connect(teeth, SIGNAL(sig_45(QPixmap*)), customLayout->scene3, SLOT(slot_45(QPixmap*)));
    connect(teeth, SIGNAL(sig_46(QPixmap*)), customLayout->scene3, SLOT(slot_46(QPixmap*)));
    connect(teeth, SIGNAL(sig_47(QPixmap*)), customLayout->scene3, SLOT(slot_47(QPixmap*)));

    /* Top Left Teeth - scene4 */
    connect(teeth, SIGNAL(sig_11(QPixmap*)), customLayout->scene4, SLOT(slot_11(QPixmap*)));
    connect(teeth, SIGNAL(sig_12(QPixmap*)), customLayout->scene4, SLOT(slot_12(QPixmap*)));
    connect(teeth, SIGNAL(sig_13(QPixmap*)), customLayout->scene4, SLOT(slot_13(QPixmap*)));
    connect(teeth, SIGNAL(sig_14(QPixmap*)), customLayout->scene4, SLOT(slot_14(QPixmap*)));
    connect(teeth, SIGNAL(sig_15(QPixmap*)), customLayout->scene4, SLOT(slot_15(QPixmap*)));
    connect(teeth, SIGNAL(sig_16(QPixmap*)), customLayout->scene4, SLOT(slot_16(QPixmap*)));
    connect(teeth, SIGNAL(sig_17(QPixmap*)), customLayout->scene4, SLOT(slot_17(QPixmap*)));

    /* Top Right Teeth - scene4 */
    connect(teeth, SIGNAL(sig_21(QPixmap*)), customLayout->scene4, SLOT(slot_21(QPixmap*)));
    connect(teeth, SIGNAL(sig_22(QPixmap*)), customLayout->scene4, SLOT(slot_22(QPixmap*)));
    connect(teeth, SIGNAL(sig_23(QPixmap*)), customLayout->scene4, SLOT(slot_23(QPixmap*)));
    connect(teeth, SIGNAL(sig_24(QPixmap*)), customLayout->scene4, SLOT(slot_24(QPixmap*)));
    connect(teeth, SIGNAL(sig_25(QPixmap*)), customLayout->scene4, SLOT(slot_25(QPixmap*)));
    connect(teeth, SIGNAL(sig_26(QPixmap*)), customLayout->scene4, SLOT(slot_26(QPixmap*)));
    connect(teeth, SIGNAL(sig_27(QPixmap*)), customLayout->scene4, SLOT(slot_27(QPixmap*)));

    /* Bottom Right Teeth - scene4 */
    connect(teeth, SIGNAL(sig_31(QPixmap*)), customLayout->scene4, SLOT(slot_31(QPixmap*)));
    connect(teeth, SIGNAL(sig_32(QPixmap*)), customLayout->scene4, SLOT(slot_32(QPixmap*)));
    connect(teeth, SIGNAL(sig_33(QPixmap*)), customLayout->scene4, SLOT(slot_33(QPixmap*)));
    connect(teeth, SIGNAL(sig_34(QPixmap*)), customLayout->scene4, SLOT(slot_34(QPixmap*)));
    connect(teeth, SIGNAL(sig_35(QPixmap*)), customLayout->scene4, SLOT(slot_35(QPixmap*)));
    connect(teeth, SIGNAL(sig_36(QPixmap*)), customLayout->scene4, SLOT(slot_36(QPixmap*)));
    connect(teeth, SIGNAL(sig_37(QPixmap*)), customLayout->scene4, SLOT(slot_37(QPixmap*)));

    /* Bottom Left Teeth - scene4 */
    connect(teeth, SIGNAL(sig_41(QPixmap*)), customLayout->scene4, SLOT(slot_41(QPixmap*)));
    connect(teeth, SIGNAL(sig_42(QPixmap*)), customLayout->scene4, SLOT(slot_42(QPixmap*)));
    connect(teeth, SIGNAL(sig_43(QPixmap*)), customLayout->scene4, SLOT(slot_43(QPixmap*)));
    connect(teeth, SIGNAL(sig_44(QPixmap*)), customLayout->scene4, SLOT(slot_44(QPixmap*)));
    connect(teeth, SIGNAL(sig_45(QPixmap*)), customLayout->scene4, SLOT(slot_45(QPixmap*)));
    connect(teeth, SIGNAL(sig_46(QPixmap*)), customLayout->scene4, SLOT(slot_46(QPixmap*)));
    connect(teeth, SIGNAL(sig_47(QPixmap*)), customLayout->scene4, SLOT(slot_47(QPixmap*)));

    /* Top Left Teeth - maxNewSc */
    connect(teeth, SIGNAL(sig_11(QPixmap*)), myMaxlayout, SLOT(slotD_11(QPixmap*)));
    connect(teeth, SIGNAL(sig_12(QPixmap*)), myMaxlayout, SLOT(slotD_12(QPixmap*)));
    connect(teeth, SIGNAL(sig_13(QPixmap*)), myMaxlayout, SLOT(slotD_13(QPixmap*)));
    connect(teeth, SIGNAL(sig_14(QPixmap*)), myMaxlayout, SLOT(slotD_14(QPixmap*)));
    connect(teeth, SIGNAL(sig_15(QPixmap*)), myMaxlayout, SLOT(slotD_15(QPixmap*)));
    connect(teeth, SIGNAL(sig_16(QPixmap*)), myMaxlayout, SLOT(slotD_16(QPixmap*)));
    connect(teeth, SIGNAL(sig_17(QPixmap*)), myMaxlayout, SLOT(slotD_17(QPixmap*)));

    /* Top Right Teeth - maxNewSc */
    connect(teeth, SIGNAL(sig_21(QPixmap*)), myMaxlayout, SLOT(slotD_21(QPixmap*)));
    connect(teeth, SIGNAL(sig_22(QPixmap*)), myMaxlayout, SLOT(slotD_22(QPixmap*)));
    connect(teeth, SIGNAL(sig_23(QPixmap*)), myMaxlayout, SLOT(slotD_23(QPixmap*)));
    connect(teeth, SIGNAL(sig_24(QPixmap*)), myMaxlayout, SLOT(slotD_24(QPixmap*)));
    connect(teeth, SIGNAL(sig_25(QPixmap*)), myMaxlayout, SLOT(slotD_25(QPixmap*)));
    connect(teeth, SIGNAL(sig_26(QPixmap*)), myMaxlayout, SLOT(slotD_26(QPixmap*)));
    connect(teeth, SIGNAL(sig_27(QPixmap*)), myMaxlayout, SLOT(slotD_27(QPixmap*)));

    /* Bottom Right Teeth - maxNewSc */
    connect(teeth, SIGNAL(sig_31(QPixmap*)), myMaxlayout, SLOT(slotD_31(QPixmap*)));
    connect(teeth, SIGNAL(sig_32(QPixmap*)), myMaxlayout, SLOT(slotD_32(QPixmap*)));
    connect(teeth, SIGNAL(sig_33(QPixmap*)), myMaxlayout, SLOT(slotD_33(QPixmap*)));
    connect(teeth, SIGNAL(sig_34(QPixmap*)), myMaxlayout, SLOT(slotD_34(QPixmap*)));
    connect(teeth, SIGNAL(sig_35(QPixmap*)), myMaxlayout, SLOT(slotD_35(QPixmap*)));
    connect(teeth, SIGNAL(sig_36(QPixmap*)), myMaxlayout, SLOT(slotD_36(QPixmap*)));
    connect(teeth, SIGNAL(sig_37(QPixmap*)), myMaxlayout, SLOT(slotD_37(QPixmap*)));

    /* Bottom Left Teeth - maxNewSc */
    connect(teeth, SIGNAL(sig_41(QPixmap*)), myMaxlayout, SLOT(slotD_41(QPixmap*)));
    connect(teeth, SIGNAL(sig_42(QPixmap*)), myMaxlayout, SLOT(slotD_42(QPixmap*)));
    connect(teeth, SIGNAL(sig_43(QPixmap*)), myMaxlayout, SLOT(slotD_43(QPixmap*)));
    connect(teeth, SIGNAL(sig_44(QPixmap*)), myMaxlayout, SLOT(slotD_44(QPixmap*)));
    connect(teeth, SIGNAL(sig_45(QPixmap*)), myMaxlayout, SLOT(slotD_45(QPixmap*)));
    connect(teeth, SIGNAL(sig_46(QPixmap*)), myMaxlayout, SLOT(slotD_46(QPixmap*)));
    connect(teeth, SIGNAL(sig_47(QPixmap*)), myMaxlayout, SLOT(slotD_47(QPixmap*)));


    /* JAE YEONG */
//---------------------------------------------
    /* Processing */
    //    ui->brightnessSlider->setRange(-100,100);
    ui->brightnessSlider->setRange(-80,80);
    ui->brightnessSlider->setSliderPosition(0);
    ui->brightnessSlider->setPageStep(1);
    ui->brightnessSlider->setTickPosition(QSlider::TicksAbove);

    ui->contrastSlider->setRange(2,6);
    ui->contrastSlider->setSliderPosition(2);
    ui->contrastSlider->setPageStep(1);
    ui->contrastSlider->setTickPosition(QSlider::TicksAbove);


//    connect(customLayout,SIGNAL(imageClear(QPixmap)),this,SLOT(sourceImage(QPixmap)));

    connect(this,SIGNAL(sig_bright(int)),ui->brightnessSlider,SLOT(setValue(int)));
    connect(this,SIGNAL(sig_dark(int)),ui->brightnessSlider,SLOT(setValue(int)));

//---------------------------------------------

/* EUN JI*/
//---------------------------------------------
    connect(customLayout,SIGNAL(sig_cursorChange()),this,SLOT(on_cursorToolButton_clicked()));

/* JAE HYEON */
//----------------------------------------------------------
    connect(customLayout->scene1, &Scene::sendLengthMeasure, this, &MainWindow::reLengthMeasure);
    connect(customLayout->scene1, &Scene::sendAngleMeasure, this, &MainWindow::reAngleMeasure);
    connect(this, &MainWindow::sendImageWHP, customLayout->scene1, &Scene::reImageWHP);

    connect(customLayout->scene2, &Scene::sendLengthMeasure, this, &MainWindow::reLengthMeasure);
    connect(customLayout->scene2, &Scene::sendAngleMeasure, this, &MainWindow::reAngleMeasure);
    connect(this, &MainWindow::sendImageWHP, customLayout->scene2, &Scene::reImageWHP);

    connect(customLayout->scene3, &Scene::sendLengthMeasure, this, &MainWindow::reLengthMeasure);
    connect(customLayout->scene3, &Scene::sendAngleMeasure, this, &MainWindow::reAngleMeasure);
    connect(this, &MainWindow::sendImageWHP, customLayout->scene3, &Scene::reImageWHP);

    connect(customLayout->scene4, &Scene::sendLengthMeasure, this, &MainWindow::reLengthMeasure);
    connect(customLayout->scene4, &Scene::sendAngleMeasure, this, &MainWindow::reAngleMeasure);
    connect(this, &MainWindow::sendImageWHP, customLayout->scene4, &Scene::reImageWHP);

    connect(myMaxlayout->maxNewSc, &Scene::sendLengthMeasure, this, &MainWindow::reLengthMeasure);
    connect(myMaxlayout->maxNewSc, &Scene::sendAngleMeasure, this, &MainWindow::reAngleMeasure);
    connect(this, &MainWindow::sendImageWHP, myMaxlayout->maxNewSc, &Scene::reImageWHP);

    ui->rulerLineEdit->setStyleSheet(sheetWhite);
    ui->angleLineEdit->setStyleSheet(sheetWhite);
//----------------------------------------------------------

    // TESTing
    //QTest::qExec(patientModel);
}

MainWindow::~MainWindow()
{
    delete ui;
    //재현
    /*프로그램 종료시 해당 폴더의 이미지 삭제*/
    QString path = "./Images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.png" << "*.jpg");    // png, jpg 파일 탐색
    dir.setFilter(QDir::Files);                                 // 파일 형태의 필터
    foreach(QString dirFile, dir.entryList()){                  // 해당 디렉토리 내의 파일 제거
        dir.remove(dirFile);
    }
}


void MainWindow::loadImages()
{
//    QDir dir(".");
    QDir dir("./Images");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->listWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon("./Images/" + fileInfoList.at(i).fileName()), NULL, ui->listWidget);
        item->setStatusTip("./Images/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
}

void MainWindow::selectItem(QListWidgetItem *item)
{
    /* view의 위치를 선택하지 않고 listWidget에 있는 이미지를 먼저 선택한 경우 */
    if(customLayout->g == false) {
        if(cnt == 0) {
            customLayout->m_currentView = customLayout->grid1;
            customLayout->m_currentScene = customLayout->scene1;
            cnt ++;
        }
        else if(cnt == 1) {
            customLayout->m_currentView = customLayout->grid2;
            customLayout->m_currentScene = customLayout->scene2;
            cnt ++;
        }
        else if(cnt == 2) {
            customLayout->m_currentView = customLayout->grid3;
            customLayout->m_currentScene = customLayout->scene3;
            cnt ++;
        }
        else if(cnt == 3) {
            customLayout->m_currentView = customLayout->grid4;
            customLayout->m_currentScene = customLayout->scene4;
            cnt ++;
        }

        foreach(auto item, customLayout->m_currentScene->items()) {
            customLayout->m_currentScene->removeItem(item);
            delete item;
        }
        customLayout->m_currentScene->setBackgroundBrush(Qt::black);
        QSize size = customLayout->m_currentView->viewport()->size();
        QGraphicsPixmapItem *pixItem = customLayout->m_currentScene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio));
        customLayout->m_currentView->setAlignment(Qt::AlignCenter);
        customLayout->m_currentScene->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->m_currentView->update();
        itemHash[pixItem] = item->statusTip();
        customLayout->m_currentView->resetTransform();
/* JAE YEONG */
//------------------------------------------------------------------------------------
        if(customLayout->m_currentScene == customLayout->scene1) {
            m_imageCopy1 = QImage(pixItem->pixmap().toImage());
            m_image1 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View1");
        }
        else if(customLayout->m_currentScene == customLayout->scene2) {
            m_imageCopy2 = QImage(pixItem->pixmap().toImage());
            m_image2 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View2");
        }
        else if(customLayout->m_currentScene ==customLayout->scene3) {
            m_imageCopy3 = QImage(pixItem->pixmap().toImage());
            m_image3 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View3");
        }
        else if(customLayout->m_currentScene ==customLayout->scene4) {
            m_imageCopy4 = QImage(pixItem->pixmap().toImage());
            m_image4 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View4");
        }
    }
//------------------------------------------------------------------------------------

    /* view의 위치를 클릭하고 이미지를 선택한 경우 */
    else if(customLayout->g == true) {
        customLayout->m_currentView->resetTransform();
        foreach(auto item, customLayout->m_currentScene->items()) {
            customLayout->m_currentScene->removeItem(item);
            delete item;
        }
        customLayout->m_currentScene->setBackgroundBrush(Qt::black);
        QSize size = customLayout->m_currentView->viewport()->size();
        QGraphicsPixmapItem *pixItem = customLayout->m_currentScene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio));
        customLayout->m_currentView->setAlignment(Qt::AlignCenter);
        customLayout->m_currentScene->setSceneRect(pixItem->sceneBoundingRect());
        customLayout->m_currentView->update();
        itemHash[pixItem] = item->statusTip();
        customLayout->m_currentView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        customLayout->m_currentView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
/* JAE YEONG */
//------------------------------------------------------------------------------------
        if(customLayout->m_currentScene == customLayout->scene1) {
            m_imageCopy1 = QImage(pixItem->pixmap().toImage());
            m_image1 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View1");
        }
        else if(customLayout->m_currentScene == customLayout->scene2) {
            m_imageCopy2 = QImage(pixItem->pixmap().toImage());
            m_image2 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View2");
        }
        else if(customLayout->m_currentScene ==customLayout->scene3) {
            m_imageCopy3 = QImage(pixItem->pixmap().toImage());
            m_image3 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View3");
        }
        else if(customLayout->m_currentScene ==customLayout->scene4) {
            m_imageCopy4 = QImage(pixItem->pixmap().toImage());
            m_image4 = QImage(pixItem->pixmap().toImage());
            pixItem->setToolTip("View4");
        }
//------------------------------------------------------------------------------------
    }
}

void MainWindow::slot_doubleSize()
{
    ui->line->hide();

    QList<int> sizes2;
    sizes2 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter2->setSizes(sizes2);

    myMaxlayout->setNewSc(customLayout->m_currentScene);

    stackWidget->setCurrentIndex(1);

    myMaxlayout->maxNewGrid->scale(size().width(), size().height());            // 주석해도될듯? (보류)
    myMaxlayout->maxNewGrid->fitInView(myMaxlayout->maxNewSc->sceneRect(),Qt::KeepAspectRatio);
    myMaxlayout->maxNewGrid->viewport()->update();
}


void MainWindow::previousScreen()
{
    stackWidget->setCurrentIndex(0);

    ui->line->show();

    QList<int> sizes2;
    sizes2 << 700 << 100;
    ui->splitter2->setSizes(sizes2);

    customLayout->grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_layoutClearPushButton_clicked()                    // scene 이미지 초기화
{
    customLayout->grid1->resetTransform();
    customLayout->grid2->resetTransform();
    customLayout->grid3->resetTransform();
    customLayout->grid4->resetTransform();

    customLayout->grid1->scene()->clear();
    customLayout->grid2->scene()->clear();
    customLayout->grid3->scene()->clear();
    customLayout->grid4->scene()->clear();

    cnt = 0;

    customLayout->g = false;

    customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
    customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
    customLayout->grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
    customLayout->grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
}



/* ui 색상 및 크기 조절 */
void MainWindow::styleColor()
{
    ui->mdiArea->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter2->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter3->setStyleSheet("background:rgb(32, 56, 100)");
    ui->splitter4->setStyleSheet("background:rgb(32, 56, 100)");
    ui->listWidget->setStyleSheet("background:rgb(255, 255, 255)");
    ui->patientTableView->setStyleSheet("background:rgb(255, 255, 255)");
    ui->doctorNameLineEdit->setStyleSheet("background:rgb(180, 199, 231)");
    //ui->line->setStyleSheet("background:rgb(255, 255, 255)");
    ui->line->setStyleSheet(sheetWhite);
    ui->logoutPushButton->setStyleSheet("background:rgb(180, 199, 231)");

    /* Painter */
    ui->tabWidget->setStyleSheet(sheetNavy);
    ui->brushToolButton->setStyleSheet(sheetWhite);
    ui->doubleSpinBox->setStyleSheet(sheetWhite);;
    ui->circleToolButton->setStyleSheet(sheetWhite);
    ui->triangleToolButton->setStyleSheet(sheetWhite);
    ui->rectangleToolButton->setStyleSheet(sheetWhite);
    ui->arrowToolButton->setStyleSheet(sheetWhite);
    ui->colorToolButton->setStyleSheet(sheetWhite);

    /* Processing */
    //ui->brightToolButton->setStyleSheet(sheetWhite);
    //ui->brightnessToolButton->setStyleSheet(sheetWhite);
    ui->sharpenToolButton->setStyleSheet(sheetWhite);
    ui->inversionToolButton->setStyleSheet(sheetWhite);
    ui->horizontalFlipToolButton->setStyleSheet(sheetWhite);
    ui->verticalFlipToolButton->setStyleSheet(sheetWhite);
    ui->zoomInToolButton->setStyleSheet(sheetWhite);
    ui->zoomOutToolButton->setStyleSheet(sheetWhite);
    ui->panningToolButton->setStyleSheet(sheetWhite);
    ui->leftRotateToolButton->setStyleSheet(sheetWhite);
    ui->rightRotateToolButton->setStyleSheet(sheetWhite);

    /* Measure */
    ui->rulerToolButton->setStyleSheet(sheetWhite);
    //ui->measureToolButton->setStyleSheet(sheetWhite);
    ui->protractorToolButton->setStyleSheet(sheetWhite);
    ui->implantToolButton->setStyleSheet(sheetWhite);
    ui->implantClearToolButton->setStyleSheet(sheetWhite);

    QList<int> sizes;
    sizes << 10 << 1000;
    ui->splitter->setSizes(sizes);

    QList<int> sizes2;
    sizes2 << 700 << 100;
    ui->splitter2->setSizes(sizes2);

    QList<int> sizes3;
    sizes3 << 10 << 2000;
    ui->splitter3->setSizes(sizes3);

    QList<int> sizes4;
    sizes4 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter4->setSizes(sizes4);


    ui->verticalSpacer->changeSize(20, 15);
    ui->verticalSpacer_2->changeSize(20, 40);
    ui->verticalSpacer_3->changeSize(20, 20);
    ui->verticalSpacer_4->changeSize(20, 20);
    ui->horizontalSpacer_8->changeSize(20, 12);
    ui->horizontalSpacer_9->changeSize(10,10);
    ui->horizontalSpacer_10->changeSize(10,10);
    ui->sourceSizePushButton->setFixedSize(70, 40);


    /* toolButton 크기 조절 */
    ui->brushToolButton->setIconSize(QSize(50,40));
    ui->brushToolButton->setText("Brush");
    ui->brushToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->colorToolButton->setIconSize(QSize(100,30));
    ui->colorToolButton->setText("Color");
    ui->colorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}




void MainWindow::on_zoomInToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentView->scale(1.25, 1.25);
        customLayout->m_currentView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->m_currentView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    myMaxlayout->maxNewGrid->scale(1.25, 1.25);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_zoomOutToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentView->scale(0.8, 0.8);
        customLayout->m_currentView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->m_currentView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    myMaxlayout->maxNewGrid->scale(0.8, 0.8);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_leftRotateToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentView->rotate(-90);
        customLayout->m_currentView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->m_currentView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    myMaxlayout->maxNewGrid->rotate(-90);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_rightRotateToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentView->rotate(90);
        customLayout->m_currentView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        customLayout->m_currentView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    myMaxlayout->maxNewGrid->rotate(90);
    myMaxlayout->maxNewGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMaxlayout->maxNewGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::on_sourceSizePushButton_clicked()
{
    if(customLayout->g == true)
        customLayout->m_currentView->resetTransform();

    myMaxlayout->maxNewGrid->resetTransform();
}

void MainWindow::slot_doctorInfo(QString DoctorID)
{
    ui->doctorNameLineEdit->setText(DoctorID);

    keyFeatures->slot_doctorInfo(DoctorID);
    keyFeatures->insertInfo(DoctorID);
//    keyFeatures->updateInfo(DoctorID);
    keyFeatures->slot_checkUpdate();

    QList<int> sizes4;
    sizes4 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter4->setSizes(sizes4);

    PatientTableLoad(); //재현
}

void MainWindow::on_logoutPushButton_clicked()
{
    qDebug() << "로그아웃 시그널";
    emit sig_logout(0);
    emit sig_logClear();
    keyFeatures->slot_checkClear();

    //재현
    on_layoutClearPushButton_clicked();
    QString path = "./Images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.png" << "*.jpg" << "*.bmp");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()){
        dir.remove(dirFile);
    }
    loadImages();
}

void MainWindow::on_implantToolButton_clicked()
{
    teeth->show();

    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Implant);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Implant);
}


void MainWindow::on_implantClearToolButton_clicked()
{
    /* scene으로 한 거 */
    if(customLayout->g == true) {
        customLayout->m_currentScene->implantClearItems();
    }
    myMaxlayout->maxNewSc->implantClearItems();
}


/* JAE YEONG */
//----------------------------------------------------------------------------------------------------------
void MainWindow::on_brightnessSlider_valueChanged(int intensity)
{
    brightness(intensity);
}


void MainWindow::on_contrastSlider_valueChanged(int alpha)
{
    contrast(alpha);
}


void MainWindow::on_sharpenToolButton_clicked()
{
    sharpen();
}


void MainWindow::on_inversionToolButton_clicked()
{
    inversion();
}


void MainWindow::on_verticalFlipToolButton_clicked()
{
    verticalFlip();
}


void MainWindow::on_horizontalFlipToolButton_clicked()
{
    horizontalFlip();
}


void MainWindow::brightness(int intensity)
{
    qDebug() << "brightness" <<intensity;
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }
        foreach (auto i, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);

            if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                    ||i->toolTip() == "View3"||i->toolTip() == "View4") {

                QImage dis_image;
                if(customLayout->m_currentScene == customLayout->scene1 ) {
                    dis_image = QImage(m_imageCopy1);
                    qDebug()<<"tooltip";
                }
                else if(customLayout->m_currentScene == customLayout->scene2) {
                    dis_image = QImage(m_imageCopy2);
                }
                else if(customLayout->m_currentScene == customLayout->scene3) {
                    dis_image = QImage(m_imageCopy3);
                }
                else if(customLayout->m_currentScene == customLayout->scene4) {
                    dis_image = QImage(m_imageCopy4);
                    qDebug()<<"bright4";
                }

                dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
                cv::Mat mat = cv::Mat(dis_image.height(),
                                      dis_image.width(),
                                      CV_8UC3,
                                      dis_image.bits(),
                                      dis_image.bytesPerLine());
                Mat brightness;
                mat.convertTo(brightness, -1, 1, intensity);

                outImg =QImage(brightness.data,
                               brightness.cols,
                               brightness.rows,
                               brightness.step,
                               QImage::Format_RGB888).copy();

                outPixImage = QPixmap::fromImage(outImg);
                pixItem->setPixmap(outPixImage);
            }
        }
    }
}

void MainWindow::brightnessButton()
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }
        foreach (auto i, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
            if(beta > 80){
                return;
            }


            if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                    ||i->toolTip() == "View3"||i->toolTip() == "View4") {
                beta += 10;

                QImage dis_image;
                if(customLayout->m_currentScene == customLayout->scene1 ) {
                    dis_image = QImage(m_imageCopy1);
                }
                else if(customLayout->m_currentScene == customLayout->scene2) {
                    dis_image = QImage(m_imageCopy2);
                }
                else if(customLayout->m_currentScene == customLayout->scene3) {
                    dis_image = QImage(m_imageCopy3);
                }
                else if(customLayout->m_currentScene == customLayout->scene4) {
                    dis_image = QImage(m_imageCopy4);
                }

                dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
                cv::Mat mat = cv::Mat(dis_image.height(),
                                      dis_image.width(),
                                      CV_8UC3,
                                      dis_image.bits(),
                                      dis_image.bytesPerLine());
                Mat brightness;
                mat.convertTo(brightness, -1, 1, beta);

                outImg =QImage(brightness.data,
                               brightness.cols,
                               brightness.rows,
                               brightness.step,
                               QImage::Format_RGB888).copy();

                outPixImage = QPixmap::fromImage(outImg);
                pixItem->setPixmap(outPixImage);
            }
            emit sig_bright(beta);
        }
    }
}

void MainWindow::darknessButton()
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }
        foreach (auto i, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
            if(beta < -80){
                return;
            }

            if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                    ||i->toolTip() == "View3"||i->toolTip() == "View4") {
                beta -= 10;

                QImage dis_image;
                if(customLayout->m_currentScene == customLayout->scene1 ) {
                    dis_image = QImage(m_imageCopy1);
                }
                else if(customLayout->m_currentScene == customLayout->scene2) {
                    dis_image = QImage(m_imageCopy2);
                }
                else if(customLayout->m_currentScene == customLayout->scene3) {
                    dis_image = QImage(m_imageCopy3);
                }
                else if(customLayout->m_currentScene == customLayout->scene4) {
                    dis_image = QImage(m_imageCopy4);
                }

                dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
                cv::Mat mat = cv::Mat(dis_image.height(),
                                      dis_image.width(),
                                      CV_8UC3,
                                      dis_image.bits(),
                                      dis_image.bytesPerLine());
                Mat brightness;
                mat.convertTo(brightness, -1, 1, beta);

                outImg =QImage(brightness.data,
                               brightness.cols,
                               brightness.rows,
                               brightness.step,
                               QImage::Format_RGB888).copy();

                outPixImage = QPixmap::fromImage(outImg);
                pixItem->setPixmap(outPixImage);
//                saveOutImage();
            }
            emit sig_dark(beta);
        }
    }
}

void MainWindow::contrast(int alpha)
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }

        foreach (auto i, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);

            if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                    ||i->toolTip() == "View3"||i->toolTip() == "View4"){

                QImage dis_image;
                if(customLayout->m_currentScene == customLayout->scene1){
                    dis_image = QImage(m_imageCopy1.copy());
                }
                else if(customLayout->m_currentScene == customLayout->scene2) {
                    dis_image = QImage(m_imageCopy2);
                }
                else if(customLayout->m_currentScene == customLayout->scene3) {
                    dis_image = QImage(m_imageCopy3);
                }
                else if(customLayout->m_currentScene == customLayout->scene4) {
                    dis_image = QImage(m_imageCopy4);
                }

                dis_image = dis_image.convertToFormat(QImage::Format_RGB888);
                cv::Mat mat = cv::Mat(dis_image.height(),
                                      dis_image.width(),
                                      CV_8UC3,
                                      dis_image.bits(),
                                      dis_image.bytesPerLine());
                Mat contrast;
                double value = (double)alpha/2;
                mat.convertTo(contrast, -1, value, 0);

                outImg = QImage(contrast.data,
                                contrast.cols,
                                contrast.rows,
                                contrast.step,
                                QImage::Format_RGB888).copy();

                outPixImage = QPixmap::fromImage(outImg);
                pixItem->setPixmap(outPixImage);
            }
        }
    }
}

void MainWindow::contrastButton()
{

    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }

        foreach (auto i, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);

            if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                    ||i->toolTip() == "View3"||i->toolTip() == "View4"){

                buttonAlpha += 0.5;
                if(m_contrastCnt == 4){
                    m_contrastCnt = 0;
                    buttonAlpha = 2;
                    if(customLayout->m_currentScene == customLayout->scene1){
                        pixItem->setPixmap(QPixmap::fromImage(m_image1.copy()));
                    }
                    else if(customLayout->m_currentScene == customLayout->scene2) {
                        pixItem->setPixmap(QPixmap::fromImage(m_image2.copy()));
                    }
                    else if(customLayout->m_currentScene == customLayout->scene3) {
                        pixItem->setPixmap(QPixmap::fromImage(m_image3.copy()));
                    }
                    else if(customLayout->m_currentScene == customLayout->scene4) {
                        pixItem->setPixmap(QPixmap::fromImage(m_image4.copy()));
                    }
                }

                QImage dis_image;
                if(customLayout->m_currentScene == customLayout->scene1){
                    dis_image = QImage(m_imageCopy1.copy());
                }
                else if(customLayout->m_currentScene == customLayout->scene2) {
                    dis_image = QImage(m_imageCopy2);
                }
                else if(customLayout->m_currentScene == customLayout->scene3) {
                    dis_image = QImage(m_imageCopy3);
                }
                else if(customLayout->m_currentScene == customLayout->scene4) {
                    dis_image = QImage(m_imageCopy4);
                }

                dis_image = dis_image.convertToFormat(QImage::Format_RGB888);
                cv::Mat mat = cv::Mat(dis_image.height(),
                                      dis_image.width(),
                                      CV_8UC3,
                                      dis_image.bits(),
                                      dis_image.bytesPerLine());
                Mat contrast;
                double value = buttonAlpha/2;
                mat.convertTo(contrast, -1, value, 0);

                outImg = QImage(contrast.data,
                                contrast.cols,
                                contrast.rows,
                                contrast.step,
                                QImage::Format_RGB888).copy();

                outPixImage = QPixmap::fromImage(outImg);
                pixItem->setPixmap(outPixImage);
                m_contrastCnt++;
            }
        }
    }
}

void MainWindow::on_contrastSlider_sliderReleased()
{
    saveOutImage();
}

void MainWindow::sharpen()
{
    inten +=0.5;

    if(customLayout->g == true){

        if(customLayout->m_currentScene->items().empty()){
            return;
        }
        if(m_sharpenCnt == 5){
            m_sharpenCnt = 0;
            inten = 0;
            qDebug()<<inten;
            if(customLayout->m_currentScene == customLayout->scene1){
                pixItem->setPixmap(QPixmap::fromImage(m_image1.copy()));
            }
            else if(customLayout->m_currentScene == customLayout->scene2) {
                pixItem->setPixmap(QPixmap::fromImage(m_image2.copy()));
            }
            else if(customLayout->m_currentScene == customLayout->scene3) {
                pixItem->setPixmap(QPixmap::fromImage(m_image3.copy()));
            }
            else if(customLayout->m_currentScene == customLayout->scene4) {
                pixItem->setPixmap(QPixmap::fromImage(m_image4.copy()));
            }
        }

        foreach (auto i, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);

            if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                    ||i->toolTip() == "View3"||i->toolTip() == "View4"){

                QPixmap pixmap = QPixmap(pixItem->pixmap());
                QImage dis_image = QImage(pixmap.toImage());

                dis_image= dis_image.convertToFormat(QImage::Format_RGB888);
                cv::Mat mat = cv::Mat(dis_image.height(),
                                      dis_image.width(),
                                      CV_8UC3,
                                      dis_image.bits(),
                                      dis_image.bytesPerLine());
                //1. 입력이미지 2.출력이미지  3.커널의 크기  4.가우스 커널 표준편차를 나타내는 double형 변수
                Mat smoothed;
                GaussianBlur(mat, smoothed,Size(5,5),0);
                Mat sharpenImage = mat + (mat- smoothed) * inten;

                outImg = QImage(sharpenImage.data,
                                sharpenImage.cols,
                                sharpenImage.rows,
                                sharpenImage.step,
                                QImage::Format_RGB888).copy();

                outPixImage = QPixmap::fromImage(outImg);
                saveOutImage();
                pixItem->setPixmap(outPixImage);
                m_sharpenCnt++;
            }
        }
    }
}

void MainWindow::inversion()
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }

        foreach(auto item, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

            if(item->toolTip() == "View1" ||item->toolTip() == "View2"
                    ||item->toolTip() == "View3"||item->toolTip() == "View4"){

                QPixmap pixmap = pixItem->pixmap();
                QImage dis_image = QImage(pixmap.toImage());

                dis_image.invertPixels();

                outPixImage = QPixmap::fromImage(dis_image);

                saveOutImage();
                pixItem->setPixmap(outPixImage);
            }
        }
    }
}

void MainWindow::verticalFlip()
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }
        foreach(auto item, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            if(item->toolTip() == "View1" ||item->toolTip() == "View2"
                    ||item->toolTip() == "View3"||item->toolTip() == "View4"){

                QPixmap pixmap = pixItem->pixmap();
                QImage dis_image = QImage(pixmap.toImage());

                dis_image.mirror(true,false);

                outPixImage = QPixmap::fromImage(dis_image);

                saveOutImage();
                pixItem->setPixmap(outPixImage);
            }
        }
    }
}

void MainWindow::horizontalFlip()
{
    if(customLayout->g == true){

        if(customLayout->m_currentScene->items().empty()){
            return;
        }
        foreach(auto item, customLayout->m_currentScene->items()){
            pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            if(item->toolTip() == "View1" ||item->toolTip() == "View2"
                    ||item->toolTip() == "View3"||item->toolTip() == "View4") {

                QPixmap pixmap = pixItem->pixmap();
                QImage dis_image = QImage(pixmap.toImage());

                dis_image.mirror(false,true);

                outPixImage = QPixmap::fromImage(dis_image);

                saveOutImage();
                pixItem->setPixmap(outPixImage);
            }
        }
    }
}

void MainWindow::on_processingClearPushButton_clicked()
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            return;
        }

        if(customLayout->m_currentScene == customLayout->scene1){
            m_imageCopy1 = m_image1.copy();
            pixItem->setPixmap(QPixmap::fromImage(m_imageCopy1));
        }
        else if(customLayout->m_currentScene == customLayout->scene2) {
            m_imageCopy2 = m_image2.copy();
            pixItem->setPixmap(QPixmap::fromImage(m_imageCopy2));
        }
        else if(customLayout->m_currentScene == customLayout->scene3) {
            m_imageCopy3 = m_image3.copy();
            pixItem->setPixmap(QPixmap::fromImage(m_imageCopy3));
        }
        else if(customLayout->m_currentScene == customLayout->scene4) {
            m_imageCopy4 = m_image4.copy();
            pixItem->setPixmap(QPixmap::fromImage(m_imageCopy4));
        }
        ui->brightnessSlider->setSliderPosition(0);
        ui->contrastSlider->setSliderPosition(2);
    }
}

void MainWindow::saveOutImage()
{
    if(customLayout->m_currentScene == customLayout->scene1){
        m_imageCopy1 = outPixImage.toImage();
    }
    else if(customLayout->m_currentScene == customLayout->scene2) {
        m_imageCopy2 = outPixImage.toImage();
    }
    else if(customLayout->m_currentScene == customLayout->scene3) {
        m_imageCopy3 = outPixImage.toImage();
    }
    else if(customLayout->m_currentScene == customLayout->scene4) {
        m_imageCopy4 = outPixImage.toImage();
    }
}

void MainWindow::on_saveToolButton_clicked()
{
    if(customLayout->g == true){
        if(customLayout->m_currentScene->items().empty()){
            QMessageBox::information(this,tr("warning"),tr("No search image"));
            return;
        }
        QMessageBox msgBox;
        QPushButton *allSave = msgBox.addButton(tr("All Save"), QMessageBox::ActionRole);
        QPushButton *processSave = msgBox.addButton(tr("ProcessSave"),QMessageBox::ActionRole);
        QPushButton *no = msgBox.addButton(tr("No"),QMessageBox::ActionRole);
        msgBox.exec();

        if (msgBox.clickedButton() == allSave) {
            // connect
            if(customLayout->g == true){
                qDebug()<<"All Save";

                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                                QString(),
                                                                tr("Images(*.png)"));
                customLayout->m_currentView->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
                customLayout->m_currentView->grab().save(fileName);
            }
            else if(customLayout->g == false){

                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                                QString(),
                                                                tr("Images(*.png)"));
                myMaxlayout->maxNewGrid->grab().save(fileName);
            }
        } else if (msgBox.clickedButton() == processSave) {

            QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                            QString(), tr("Images(*.png)"));
            foreach(auto i, customLayout->m_currentScene->items()){
                pixItem = dynamic_cast<QGraphicsPixmapItem*>(i);
                if(i->toolTip() == "View1" ||i->toolTip() == "View2"
                        ||i->toolTip() == "View3"||i->toolTip() == "View4"){
                    pixItem->pixmap().save(fileName);
                }
            }
        }
        else if(msgBox.clickedButton() == no){
            return;
        }
    }
}


/* EUN JI */
//----------------------------------------------------------------------
void MainWindow::on_cursorToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Cursor);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Cursor);

    setCursor(Qt::ArrowCursor);
}

void MainWindow::on_brushToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Path);
    }

    myMaxlayout->maxNewSc->setCurrentShape(Scene::Path);

    QCursor cursorBrush = QCursor(QPixmap("pen5.png"), 0,0);
    setCursor(cursorBrush); // Set the cursor
}


void MainWindow::on_circleToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Ellipse);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Ellipse);
}


void MainWindow::on_triangleToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Triangle);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Triangle);
}


void MainWindow::on_rectangleToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Rect);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Rect);
}


void MainWindow::on_arrowToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Arrow);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Arrow);
}


void MainWindow::on_lineToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Line);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Line);
}


void MainWindow::on_colorToolButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()){
        if(customLayout->g == true) {
            customLayout->m_currentScene->setCurrentColor(color);
        }
        myMaxlayout->maxNewSc->setCurrentColor(color);
    }
}


void MainWindow::on_doubleSpinBox_valueChanged(double size)
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setPaintSize(size);
    }
    myMaxlayout->maxNewSc->setPaintSize(size);
}


void MainWindow::on_drawClearPushButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->clearItems();
    }
    myMaxlayout->maxNewSc->clearItems();
}


void MainWindow::on_deletePushButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->deleteItems();
    }
    myMaxlayout->maxNewSc->deleteItems();
}


void MainWindow::on_printPushButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Text);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Text);

    QString text = ui->textEdit->toPlainText();

    if(customLayout->g == true) {
        customLayout->m_currentScene->setText(text);
    }
    myMaxlayout->maxNewSc->setText(text);

}

void MainWindow::on_rotateDoubleSpinBox_valueChanged(double angle)
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setRotation(angle);
    }
    myMaxlayout->maxNewSc->setRotation(angle);

    if(customLayout->g == true) {
        customLayout->m_currentScene->rotate();
    }
    myMaxlayout->maxNewSc->rotate();
}

/* JAE HY */
//-------------------------------------------------------------------------
/*길이 측정 버튼 함수*/
void MainWindow::on_rulerToolButton_clicked()
{
    if(customLayout->g == true) {       // 선택된 grid가 있으면
        customLayout->m_currentScene->setCurrentShape(Scene::Length);   // 길이 측정 모양 호출
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Length);              // 큰 화면일때도 길이 측정 모양 호출
}

/*각도 측정 버튼 함수*/
void MainWindow::on_protractorToolButton_clicked()
{
    if(customLayout->g == true) {       // 선택된 grid가 있으면
        customLayout->m_currentScene->setCurrentShape(Scene::Angle);    // 각도 측정 모양 호출
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Angle);               // 큰 화면일때도 각도 측정 모양 호출
}

/*블랜딩 버튼 클릭시 작동하는 함수*/
void MainWindow::on_blendingPushButton_clicked()
{
    QDir dir("./Images/");
    /*./Image 디렉토리가 없을경우*/
    if(dir.isEmpty()){
        QMessageBox::critical(this, "Blending Dialog",
                                 "Do Not any show listWidget");
        /*./Image 디렉토리의 내용이 없다고 알리며 예외처리*/
        return;
    }else{
        /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
        QMessageBox::StandardButton buttonReply;

        /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
        buttonReply = QMessageBox::question(this, "question", "Are you Open Blending Dialog?",
                                            QMessageBox::Yes | QMessageBox::No);

        /*버튼을 누를 시 Yes인 경우*/
        if(buttonReply == QMessageBox::Yes) {
            /*blendDialog 호출*/
            blending = new BlendingDlg(this);
            blending->exec();
        } else {
            return;         // 호출하지 않음
        }
    }
}

/*환자 테이블 불러오기 함수*/
void MainWindow::PatientTableLoad()
{
    /*환자 데이터를 리스트화*/
    QList<QString> contacName;
    QList<int> age;
    QList<QString> doctorID;

    /*환자 정보 Url 검색*/
    QNetworkRequest req( QUrl( QString("http://" + hostName + ":" + portNum + "/api/patient/") ) );
    reply = manager->get(req);
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

    //에러메시지가 없는 경우
    if (reply->error( ) == QNetworkReply::NoError) {
        /*해당 Url에 저장된 Json데이터를 읽은 후 response배열 내부의 jsonData Parsing*/
        QString strReply = (QString)reply->readAll( );
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8( ));
        QJsonArray jsonArr = jsonResponse["response"].toArray();
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();    //jsonResponse.object();
            QString ID = patientObj["_id"].toString();              // 환자 아이디 변수 선언
            QString Name = patientObj["Name"].toString();           // 환자 성함 변수 선언
            int Age = patientObj["Age"].toInt();                    // 환자 나이 변수 선언
            QString DoctorID = patientObj["DoctorID"].toString();   // 환자의 담당의사 로그인 변수 선언

            /*로그인 된 DoctorID에 해당하는 환자들만 테이블에 나열*/
            if(ui->doctorNameLineEdit->text() == DoctorID) {
                contacName.append(Name);
                age.append(Age);
                doctorID.append(DoctorID);
            }
        }
        delete reply;
        /*환자모델 데이터를 얻은 후 PatientModel 클래스에 환자데이터 삽입*/
        patientModel = new PatientModel(this);
        patientModel->patientData(contacName, age, doctorID);       // 환자의 모델 데이터 입력
        ui->patientTableView->setModel(patientModel);               // patientModel 설정
        ui->patientTableView->show();                               // Model 출력
    }
}

/*환자 테이블 클릭시 발생하는 이벤트 함수*/
void MainWindow::on_patientTableView_clicked(const QModelIndex &index)
{
    //해당환자의 테이블을 클릭시 이미지 폴더의 png 파일들을 제거
    QString path = "./Images";                                  // path 경로 탐색
    QDir dir(path);                                             // dir 변수 초기화
    dir.setNameFilters(QStringList() << "*.png" << "*.jpg");    // png 파일 리스트
    dir.setFilter(QDir::Files);                                 // 파일 데이터 필터화
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);                                    // 디렉토리 내부의 파일 제거
    }

    QString patient = index.data().toString();                  // 환자의 이름을 받는 변수 할당
    patView = new PatitentView;                                 // 해당 환자의 이미지 다운로드
    connect(patView, &PatitentView::middlePatient, this, &MainWindow::receiveDownload);     // 중계 연결
    //환자의 테이블을 나열하기위한 함수 호출 후 환자의 이름을 입력받으면 해당이름에 맞는 이미지 리스트 다운로드
    patView->patientView(hostName, portNum, patient);
}

/*다운로드 완료 시 시그널을 받는 슬롯 함수*/
void MainWindow::receiveDownload()
{
    loadImages();       // listWidget의 이미지들을 나열
}

/*클라이언트에서 처리한 이미지를 서버로 업로드 하는 함수*/
void MainWindow::on_uploadButton_clicked()
{
    /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
    QMessageBox::StandardButton buttonReply;

    /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
    buttonReply = QMessageBox::question(this, "question", "Are you Open Upload Dialog?",
                                        QMessageBox::Yes | QMessageBox::No);

    /*버튼을 누를 시 Yes인 경우*/
    if(buttonReply == QMessageBox::Yes) {
        /*blendDialog 호출*/
        uploading = new UploadDlg(this);
        uploading->exec();
        PatientTableLoad();

    }
    else {
        /*호출하지 않음*/
        return;
    }
}

/*리스트 위젯의 이미지를 더블 클릭시 삭제하는 함수(좌/우 클릭 모두 가능)*/
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    /*파일정보 리스트에 png, jpg, bmp파일들만 리스트정보에 넣어둠*/
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    /*png 파일을 fileName으로 */
    QString fileName = fileInfoList.at(index.row()).fileName(); //*.png
    /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
    QMessageBox::StandardButton buttonReply;

    /*이미지를 삭제한다는 의항이 있냐는 메세지 박스를 출력*/
    buttonReply = QMessageBox::question(this, "question", "Are you Delete this image?",
                                        QMessageBox::Yes | QMessageBox::No);

    /*버튼을 누를 시 Yes인 경우*/
    if(buttonReply == QMessageBox::Yes) {
        delImage = new DeleteImage;
        /*삭제 URL과 삭제할 파일의 이름을 파라미터로 데이터를 보냄*/
        delImage->deleteImage(hostName, portNum, fileName);
        loadImages();
    } else {
        return;      // 호출하지 않음
    }
}

/*길이 측정 버튼 함수*/
void MainWindow::reLengthMeasure(double length)
{
    ui->rulerLineEdit->setText(QString::number(length) + " mm");  // Scene으로 부터 측정된 결과값을 라인에디트에 출력
}
/*각도 측정 버튼 함수*/
void MainWindow::reAngleMeasure(double angle)
{
    ui->angleLineEdit->setText(QString::number(angle) + " º"); // Scene으로 부터 측정된 결과값을 라인에디트에 출력
}


/*이미지 클릭시 해당 이미지의 픽셀 및 폭, 넓이 값 전달*/
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    QDir dir("./Images/");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    /*파일정보 리스트에 png, jpg, bmp파일들만 리스트정보에 넣어둠*/
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    /*png 파일을 baseName으로 */
    QString fileName = fileInfoList.at(index.row()).fileName(); //*.png

    /*Url 탐색후 해당 이미지의 픽셀당 길이 정보를 추출*/
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QNetworkRequest req( QUrl( QString("http://" + hostName + ":" + portNum + "/api/image/") ) );
    QNetworkReply *reply = manager.get(req);
    connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec( );           // "finished( )" 가 호출 될때까지 블록(동기화)

    /*해당 폴더에 저장된 이미지의 사이즈를 출력*/
    QImageReader reader("./Images/"+fileName);      // 이미지파일 포함 경로 변수
    QSize sizeOfImage = reader.size();              // 이미지의 크기
    int height = sizeOfImage.height();              // 이미지 크기 중 height 값 추출
    int width = sizeOfImage.width();                // 이미지 크기 중 width 값 추출
    if (reply->error( ) == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll( );  // 전체 이미지 데이터 읽기

        QJsonDocument jsonResponse =
                QJsonDocument::fromJson(strReply.toUtf8( ));        // Json 데이터 포멧

        QJsonArray jsonArr = jsonResponse["response"].toArray();    // response Array 내부의 JsonData Parsing
        for(int i = 0; i < jsonArr.size(); i++) {
            QJsonObject patientObj = jsonArr.at(i).toObject();      // jsonResponse.object();
            QString ImageFile = patientObj["ImageFile"].toString();
            double ImagePixel = patientObj["PixelLength"].toDouble();

            QString csvString = ImageFile.section("\\", 1, 1);      // \\uploads/*.png => *.png로 파싱
            if(fileName == csvString){                              // 해당 이미지를 찾은 경우
                emit sendImageWHP(width, height, ImagePixel);       // 이미지 정보 전송
            }
        }
        delete reply;
    }
}

