#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layout.h"
#include "maxlayout.h"
#include "keyfeaturesform.h"
#include "teethform.h"
#include "view.h"
#include "scene.h"
#include "blendingdlg.h"
#include "uploaddlg.h"
#include "patientmodel.h"
#include "patientview.h"
#include "deleteimage.h"

#include <opencv2/opencv.hpp>
using namespace cv;


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
#include <QMenu>

/*JAE HYEON*/
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QImageReader>

/* JAE YEONG*/
#include <QFileDialog>
#include <QMessageBox>

#define sheetWhite "background:rgb(255, 255, 255)"
#define sheetNavy "background:rgb(32, 56, 100)"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    
    QDir saveDir;
    saveDir.mkdir("./SaveImage");
    ui->setupUi(this);

    stackWidget = new QStackedWidget(this); // QStackedWidget 할당
    stackWidget->setWindowTitle(tr("2 X 2 Layout"));
    connect(stackWidget, SIGNAL(destroyed()), stackWidget, SLOT(deleteLater()));

    teeth = new TeethForm();    // TeethForm 할당
    connect(teeth, SIGNAL(destroyed()), teeth, SLOT(deleteLater()));

    customLayout = new Layout(this);    // Layout 할당
    connect(customLayout, SIGNAL(destroyed()), customLayout, SLOT(deleteLater()));

    keyFeatures = new KeyFeaturesForm(this);    // KeyFeaturesForm 할당
    connect(keyFeatures, SIGNAL(destroyed()), keyFeatures, SLOT(deleteLater()));

    myMaxlayout = new Maxlayout(this);  // Maxlayout 할당
    connect(myMaxlayout, SIGNAL(destroyed()), myMaxlayout, SLOT(deleteLater()));

    /*네트워크 접근을 위한 manager 변수 new 할당 eventLoop로 서버 동기화*/
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));

    stackWidget->insertWidget(0, customLayout); // 인덱스 0 : Layout
    stackWidget->insertWidget(1, myMaxlayout);  // 인덱스 1 : Maxlayout
    stackWidget->setCurrentIndex(0);    // 현재 인덱스는 0이므로 Layout 실행

    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(stackWidget);
    stackWidget->showMaximized();
    cw->setStyleSheet("background:rgb(0, 0, 0)");
    ui->mdiArea->setActiveSubWindow(cw);

    QVBoxLayout *KeyLayout = new QVBoxLayout();
    KeyLayout->addWidget(keyFeatures);
    ui->frame->setLayout(KeyLayout);

    styleColor();   // ui 색상 조절 및 크기 조절 함수

    ui->listWidget->setIconSize(QSize(130, 80));    // listWidget Icon 크기 설정
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setFlow(QListWidget::LeftToRight);  // listWidget Icon 왼쪽 정렬
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*))); // listWidget에서 item을 클릭했을 때 selectItem 슬롯 호출
    connect(customLayout, SIGNAL(sig_widgetbyDClick()), SLOT(slot_doubleSize()));   // 레이아웃을 더블 클릭했을 때 slot_doubleSize 슬롯 호출
    connect(myMaxlayout->viewQuit, SIGNAL(clicked()), SLOT(previousScreen()));  // viewQuit 버튼을 클릭했을 때 previousScreen 슬롯 호출
    connect(myMaxlayout, SIGNAL(sig_clicked(QPointF)), customLayout, SLOT(showMenu(QPointF)));  // 마우스 가운데 버튼을 클릭한 좌표에 showMenu 함수 출력

    /* Key Features 기능에 관한 시그널 슬롯 */
    connect(keyFeatures, SIGNAL(sig_keyCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)),
            customLayout, SLOT(slot_keyCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)));
    connect(keyFeatures, SIGNAL(sig_logCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)),
            customLayout, SLOT(slot_logCheck(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)));
    connect(keyFeatures, SIGNAL(sig_save(bool)), customLayout, SLOT(slot_save(bool)));

    connect(customLayout->zoomInAction, SIGNAL(triggered()), SLOT(on_zoomInToolButton_clicked()));  // 확대 기능 선택 시 on_zoomInToolButton_clicked 슬롯 호출
    connect(customLayout->zoomOutAction, SIGNAL(triggered()), SLOT(on_zoomOutToolButton_clicked()));    //축소 기능 선택 시 on_zoomOutToolButton_clicked 슬롯 호출
    connect(customLayout->rightRotateAction, SIGNAL(triggered()), SLOT(on_rightRotateToolButton_clicked()));    // 90°회전 선택 시 on_rightRotateToolButton_clicked 슬롯 호출
    connect(customLayout->leftRotateAction, SIGNAL(triggered()), SLOT(on_leftRotateToolButton_clicked()));  // -90°회전 선택 시 on_leftRotateToolButton_clicked 슬롯 호출
    connect(customLayout->brushAction, SIGNAL(triggered()), SLOT(on_brushToolButton_clicked()));    // 브러쉬 기능 선택 시 on_brushToolButton_clicked 슬롯 호출
    connect(customLayout->rectangleAction, SIGNAL(triggered()), SLOT(rectangleButton()));    // 사각형 기능 선택 시 on_rectangleToolButton_clicked 슬롯 호출
    connect(customLayout->triangleAction, SIGNAL(triggered()), SLOT(triangleButton()));  // 삼각형 기능 선택 시 on_triangleToolButton_clicked 슬롯 호출
    connect(customLayout->ellipseAction, SIGNAL(triggered()), SLOT(circleButton())); // 원형 기능 선택 시 on_circleToolButton_clicked 슬롯 호출
    connect(customLayout->cursorAction, SIGNAL(triggered()), SLOT(on_cursorToolButton_clicked()));  // 커서 기능 선택 시 on_cursorToolButton_clicked 슬롯 호출
    connect(customLayout->blendingAction, SIGNAL(triggered()), SLOT(on_blendingPushButton_clicked()));  // 블랜딩 기능 선택 시 on_blendingPushButton_clicked 슬롯 호출
    connect(customLayout->lengthMeasurementAction, SIGNAL(triggered()), SLOT(on_rulerToolButton_clicked()));    // 길이 측정 선택 시 on_rulerToolButton_clicked 슬롯 호출
    connect(customLayout->angleMeasurementAction, SIGNAL(triggered()), SLOT(on_protractorToolButton_clicked()));    // 각도 측정 선택 시 on_protractorToolButton_clicked 슬롯 호출
    connect(customLayout->brightnessAction, SIGNAL(triggered()), SLOT(brightnessButton())); // 밝기 기능 선택 시 brightnessButton 슬롯 호출
    connect(customLayout->darknessAction, SIGNAL(triggered()), SLOT(darknessButton())); // 어둡기 기능 선택 시 darknessButton 슬롯 호출
    connect(customLayout->sharpenAction, SIGNAL(triggered()), SLOT(on_sharpenToolButton_clicked()));    // 선명화 기능 선택 시 on_sharpenToolButton_clicked 슬롯 호출
    connect(customLayout->contrastAction, SIGNAL(triggered()), SLOT(contrastButton())); // 명암 기능 선택 시 contrastButton 슬롯 호출
    connect(customLayout->inversionAction, SIGNAL(triggered()), SLOT(on_inversionToolButton_clicked()));    // 대비 기능 선택 시 on_inversionToolButton_clicked 슬롯 호출
    connect(customLayout->horizontalFlipAction, SIGNAL(triggered()), SLOT(on_horizontalFlipToolButton_clicked()));  // 수평 반전 기능 선택 시 on_horizontalFlipToolButton_clicked 슬롯 호출
    connect(customLayout->verticalFlipAction, SIGNAL(triggered()), SLOT(on_verticalFlipToolButton_clicked()));  // 수직 반전 기능 선택 시 on_verticalFlipToolButton_clicked 슬롯 호출
    connect(customLayout->implantAction, SIGNAL(triggered()), SLOT(on_implantPushButton_clicked()));    // 임플란트 기능 선택 시 on_implantToolButton_clicked 슬롯 호출
    connect(customLayout->sourceSizeAction, SIGNAL(triggered()), SLOT(on_actionSource_Size_triggered()));  // 원본 크기 기능 선택 시 on_sourceSizePushButton_clicked 슬롯 호출
    connect(customLayout->layoutImageClearAction, SIGNAL(triggered()), SLOT(on_actionLayout_Clear_triggered()));   // 레이아웃 초기화 기능 선택 시 on_layoutClearPushButton_clicked 슬롯 호출
    connect(customLayout->drawClearAction, SIGNAL(triggered()), SLOT(on_actionDraw_Clear_triggered()));    // Draw 초기화 기능 선택 시 on_drawClearPushButton_clicked 슬롯 호출
    connect(customLayout->implantClearAction, SIGNAL(triggered()), SLOT(on_actionImplant_Clear_triggered()));  // 임플란트 초기화 기능 선택 시 on_implantClearToolButton_clicked 슬롯 호출
    connect(customLayout->imageProcessingClearAction, SIGNAL(triggered()), SLOT(on_actionImage_Processing_Clear_triggered()));   // Image Processing 초기화 선택 시 on_processingClearPushButton_clicked 슬롯 호출
    connect(customLayout->arrowAction, SIGNAL(triggered()), SLOT(arrowButton()));    // 화살표 기능 선택 시 on_arrowToolButton_clicked 슬롯 호출
    connect(customLayout->lineAction, SIGNAL(triggered()), SLOT(lineButton()));  // 선 기능 선택 시 on_lineToolButton_clicked 슬롯 호출

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
    ui->brightnessSlider->setRange(-80,80);
    ui->brightnessSlider->setSliderPosition(0);
    ui->brightnessSlider->setPageStep(1);
    ui->brightnessSlider->setTickPosition(QSlider::TicksAbove);

    ui->contrastSlider->setRange(2,6);
    ui->contrastSlider->setSliderPosition(2);
    ui->contrastSlider->setPageStep(1);
    ui->contrastSlider->setTickPosition(QSlider::TicksAbove);

    connect(this,SIGNAL(sig_bright(int)),ui->brightnessSlider,SLOT(setValue(int)));
    connect(this,SIGNAL(sig_dark(int)),ui->brightnessSlider,SLOT(setValue(int)));

//---------------------------------------------


/* EUN JI*/
//---------------------------------------------
    connect(customLayout,SIGNAL(sig_cursorChange()),this,SLOT(on_cursorToolButton_clicked()));

    shapemenu = new QMenu(this);

    circleAction = new QAction("Circle",this);
    circleAction->setIcon(QIcon(":/iii/iii/ellipse.png"));
    rectangleAction = new QAction("Rectangle",this);
    rectangleAction->setIcon(QIcon(":/iii/iii/rectangle2.png"));
    triangleAction = new QAction("Triangle",this);
    triangleAction->setIcon(QIcon(":/iii/iii/triangle.png"));
    lineAction = new QAction("Line",this);
    lineAction->setIcon(QIcon(":/iii/iii/line.png"));
    arrowAction = new QAction("Arrow",this);
    arrowAction->setIcon(QIcon(":/iii/iii/arrow.png"));

    shapemenu->addAction(circleAction);
    shapemenu->addAction(rectangleAction);
    shapemenu->addAction(triangleAction);
    shapemenu->addAction(lineAction);
    shapemenu->addAction(arrowAction);

    QObject::connect(circleAction, SIGNAL(triggered()),this, SLOT(circleButton()));
    QObject::connect(rectangleAction, SIGNAL(triggered()),this, SLOT(rectangleButton()));
    QObject::connect(triangleAction, SIGNAL(triggered()),this, SLOT(triangleButton()));
    QObject::connect(lineAction, SIGNAL(triggered()),this, SLOT(lineButton()));
    QObject::connect(arrowAction, SIGNAL(triggered()),this, SLOT(arrowButton()));


    ui->shapeToolButton->setMenu(shapemenu);
    ui->shapeToolButton->setPopupMode(QToolButton::InstantPopup);   // 도형 버튼 화살표
    ui->doubleSpinBox->setValue(3.0f);

//---------------------------------------------


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
//----------------------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* listWidget에 파일 로드하는 함수 */
void MainWindow::loadImages()
{
    QDir dir("./Images");  // 경로
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->listWidget->clear();
    for(int i = 0; i < fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon("./Images/" + fileInfoList.at(i).fileName()), NULL, ui->listWidget);
        item->setStatusTip("./Images/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
}

/* listWidget에서 이미지를 선택할 때 호출되는 슬롯 */
void MainWindow::selectItem(QListWidgetItem *item)
{
    /* view의 위치를 선택하지 않고 listWidget에 있는 이미지를 먼저 선택한 경우 - grid1, 2, 3, 4 순서대로 이미지가 출력됨 */
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
        customLayout->m_currentScene->setBackgroundBrush(Qt::black);    // m_currentScene의 배경색 설정
        QSize size = customLayout->m_currentView->viewport()->size();   // m_currentView의 크기
        QGraphicsPixmapItem *pixItem = customLayout->m_currentScene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio)); // 선택된 이미지를 m_currentView의 크기에 맞춰서 정렬
        customLayout->m_currentView->setAlignment(Qt::AlignCenter); // m_currentView 가운데 정렬
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

    /* view의 위치를 클릭하고 이미지를 선택한 경우 - 선택된 view에서만 이미지가 출력 */
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

/* 레이아웃에 있는 view 1,2,3,4를 더블 클릭했을 때 실행되는 슬롯 */
void MainWindow::slot_doubleSize()
{
    ui->line->hide();

    QList<int> sizes2;
    sizes2 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter2->setSizes(sizes2);

    myMaxlayout->setNewSc(customLayout->m_currentScene);    // maxNewSc애 m_currentScene을 대입

    stackWidget->setCurrentIndex(1);    // 인덱스가 1 이므로 Maxlayout 실행

    myMaxlayout->maxNewGrid->scale(size().width(), size().height());
    myMaxlayout->maxNewGrid->fitInView(myMaxlayout->maxNewSc->sceneRect(),Qt::KeepAspectRatio);
    myMaxlayout->maxNewGrid->viewport()->update();
}

/* viewQuit(x버튼)을 눌렀을 때 실행되는 슬롯 */
void MainWindow::previousScreen()
{
    stackWidget->setCurrentIndex(0);    // 인덱스가 0 이므로 Layout 실행

    ui->line->show();

    QList<int> sizes2;
    sizes2 << 700 << 100;
    ui->splitter2->setSizes(sizes2);

    /* 스크롤바 제거 */
    customLayout->grid1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    customLayout->grid4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/* 레이아웃에 있는 view1, 2, 3, 4에 있는 모든 Scene이 초기화 됨 */
void MainWindow::on_actionLayout_Clear_triggered()
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

    customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
    customLayout->grid1->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
    customLayout->grid3->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");
    customLayout->grid4->setStyleSheet("border: 0.5px solid rgb(129, 134, 143)");

    customLayout->g = false;
}



/* ui 색상 및 크기 조절 */
void MainWindow::styleColor()
{
    ui->toolBar->setStyleSheet("background-color: rgb(32, 56, 100);"
                               "color: rgb(255, 255, 255);");
    ui->line->setStyleSheet("border: 1.5px dashed rgb(255, 255, 255)");

    ui->patientTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->patientTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QList<int> sizes;
    sizes << 10 << 1000;
    ui->splitter->setSizes(sizes);

    QList<int> sizes2;
    sizes2 << 700 << 100;
    ui->splitter2->setSizes(sizes2);

    QList<int> sizes3;
    sizes3 << 20 << 2000;
    ui->splitter3->setSizes(sizes3);

    QList<int> sizes4;
    sizes4 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter4->setSizes(sizes4);


    ui->verticalSpacer->changeSize(20, 15);
    ui->horizontalSpacer_9->changeSize(10,10);
    ui->horizontalSpacer_10->changeSize(10,10);
}

/* 확대 기능 선택 시 실행되는 슬롯 */
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

/* 축소 기능 선택 시 실행되는 슬롯 */
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

/* 90°회전 기능 선택 시 실행되는 슬롯 */
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

/* -90°회전 기능 선택 시 실행되는 슬롯 */
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

/* 원본 크기 기능 선택 시 실행되는 슬롯 */
void MainWindow::on_actionSource_Size_triggered()
{
    if(customLayout->g == true)
        customLayout->m_currentView->resetTransform();

    myMaxlayout->maxNewGrid->resetTransform();
    myMaxlayout->maxNewGrid->fitInView(myMaxlayout->maxNewSc->sceneRect(),Qt::KeepAspectRatio);
    myMaxlayout->maxNewGrid->viewport()->update();
}


/* 로그인 할 때 실행되는 슬롯 */
void MainWindow::slot_doctorInfo(QString DoctorID)
{
    ui->doctorNameLineEdit->setText(DoctorID);  // doctorNameLineEdit을 인자로 받은 DoctorID로 설정

    keyFeatures->slot_doctorInfo(DoctorID); // slot_doctorInfo 슬롯 호출
    keyFeatures->insertInfo(DoctorID);  // insertInfo 슬롯 호출
    keyFeatures->slot_checkUpdate();    // slot_checkUpdate 슬롯 호출

    QList<int> sizes4;
    sizes4 << Qt::MaximumSize << Qt::MinimumSize;
    ui->splitter4->setSizes(sizes4);
    PatientTableLoad();             // 로그인 시 해당 환자를 나열
}

/* 로그아웃 할 때 실행되는 슬롯 */
void MainWindow::on_logoutPushButton_clicked()
{
    emit sig_logout(0); // 로그인 페이지로 돌아가는 시그널
    emit sig_logClear();
    keyFeatures->slot_checkClear(); // slot_checkClear 슬롯 호출
    qobject_cast<QWidget*>(parent())->setGeometry(570 ,170, 600, 600);

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

    QString path = "./Images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.png" << "*.jpg" << "*.bmp");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()){
        dir.remove(dirFile);
    }
    loadImages();
}

/* 임플란트 기능 선택 시 실행되는 슬롯 */
void MainWindow::on_implantPushButton_clicked()
{
    teeth->show();  // 임플란트 종류를 선택할 수 있음

    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Implant);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Implant);
}

/* 임플란트 초기화 기능 선택 시 실행되는 슬롯 */
void MainWindow::on_actionImplant_Clear_triggered()
{
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
                m_contrastCnt ++;
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

void MainWindow::on_actionImage_Processing_Clear_triggered()
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

void MainWindow::on_actionSave_triggered()
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

    QCursor cursorBrush = QCursor(QPixmap("pen4.png"), 0,0);
    setCursor(cursorBrush); // Set the cursor
}


void MainWindow::circleButton()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Ellipse);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Ellipse);
}

void MainWindow::triangleButton()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Triangle);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Triangle);
}

void MainWindow::rectangleButton()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Rect);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Rect);
}

void MainWindow::arrowButton()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Arrow);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Arrow);
}

void MainWindow::lineButton()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Line);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Line);
}

void MainWindow::on_colorPushButton_clicked()
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

void MainWindow::on_actionDraw_Clear_triggered()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->clearItems();
    }
    myMaxlayout->maxNewSc->clearItems();
}

void MainWindow::deleteButton()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->deleteItems();
    }
    myMaxlayout->maxNewSc->deleteItems();
}

void MainWindow::on_memoToolButton_clicked()
{
//    if(customLayout->g == true) {
//        customLayout->m_currentScene->setCurrentShape(Scene::Text);
//    }
//    myMaxlayout->maxNewSc->setCurrentShape(Scene::Text);

//    QString text = ui->textEdit->toPlainText();

//    if(customLayout->g == true) {
//        customLayout->m_currentScene->setText(text);
//    }
//    myMaxlayout->maxNewSc->setText(text);
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
                contacName.append(Name);                            // 환자 성함 등록
                age.append(Age);                                    // 환자 나이 등록
                doctorID.append(DoctorID);                          // 담당의사 아이디 등록
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

void MainWindow::on_rulerToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Length);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Length);
}


void MainWindow::on_protractorToolButton_clicked()
{
    if(customLayout->g == true) {
        customLayout->m_currentScene->setCurrentShape(Scene::Angle);
    }
    myMaxlayout->maxNewSc->setCurrentShape(Scene::Angle);
}


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
            /*호출하지 않음*/
            return;
        }
    }
}

/*다운로드 완료 시 시그널을 받는 슬롯 함수*/
void MainWindow::receiveDownload()
{
    loadImages();       // listWidget의 이미지들을 나열
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
    patView = new PatientView;                                 // 해당 환자의 이미지 다운로드
    connect(patView, &PatientView::middlePatient, this, &MainWindow::receiveDownload);     // 중계 연결
    //환자의 테이블을 나열하기위한 함수 호출 후 환자의 이름을 입력받으면 해당이름에 맞는 이미지 리스트 다운로드
    patView->patientView(hostName, portNum, patient);
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

/*클라이언트에서 처리한 이미지를 서버로 업로드 하는 함수*/
void MainWindow::on_actionUpload_triggered()
{
    /*Yes/No 버튼을 누를 시 다이얼로그를 호출하는 버튼 변수 생성*/
    QMessageBox::StandardButton buttonReply;

    /*Blending Dialog를 띄울 사항이 있냐는 메세지 박스를 출력*/
    buttonReply = QMessageBox::question(this, "question", "Are you Open Upload Dialog?",
                                        QMessageBox::Yes | QMessageBox::No);

    /*버튼을 누를 시 Yes인 경우*/
    if(buttonReply == QMessageBox::Yes) {
        /*blendDialog 호출*/
        uploading = new UploadDlg(this);        // 이미지 업로드
        uploading->exec();                      // 다이얼로그 호출
        PatientTableLoad();                     // 완료시 리스트 위젯 이미지 업로드

    }
    else {
        /*호출하지 않음*/
        return;
    }
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

