#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>

class Layout;

class Menuitem : public QWidget
{
    Q_OBJECT
public:
    explicit Menuitem(QWidget *parent = nullptr);


    QMenu* menu;


    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* leftRotateAction;
    QAction* rightRotateAction;
    QAction* brushAction;
    QAction* rectangleAction;
    QAction* triangleAction;
    QAction* ellipseAction;
    QAction* moveShapeAction;
    QAction* blendingAction;
    QAction* lengthMeasurementAction;
    QAction* angleMeasurementAction;
    QAction* brightnessAction;
    QAction* darknessAction;
    QAction* sharpenAction;
    QAction* contrastAction;
    QAction* inversionAction;
    QAction* horizontalFlipAction;
    QAction* verticalFlipAction;
    QAction* implantAction;
    QAction* layoutImageClearAction;
    QAction* sourceSizeAction;
    QAction* brushClearActionAction;
    QAction* imageProcessingClearAction;


//public slots:
//    void slot_zoomIn();


private:
    Layout *customLayout;


signals:

};

#endif // MENUITEM_H
