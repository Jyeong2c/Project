#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QWidget>

class Layout;

class NewWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NewWindow(QWidget *parent = nullptr);

    Layout *customLyaout;

signals:

};

#endif // NEWWINDOW_H
