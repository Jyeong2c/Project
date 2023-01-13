#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);

public slots:
    void bright();
    void bright2();


private:
    void gridLayout();
    QImage * image;
    QGraphicsView *grid1;
    QGraphicsScene* scene;

signals:

};

#endif // LAYOUT_H
