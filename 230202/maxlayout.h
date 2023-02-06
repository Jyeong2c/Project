#ifndef MAXLAYOUT_H
#define MAXLAYOUT_H

#include <QWidget>

class QHBoxLayout;
class QGraphicsView;
class QGraphicsScene;
class QPushButton;

class Maxlayout : public QWidget
{
    Q_OBJECT
public:
    explicit Maxlayout(QWidget *parent = nullptr);

    QGraphicsView *maxNewGrid;
    QGraphicsScene *maxNewSc;
    QPushButton *viewQuit;

    QPointF clickPoint;

    void setNewSc(QGraphicsScene*);
    void setNewGrid(QGraphicsView*);



protected:
    void resizeEvent(QResizeEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    QWidget *maxWidget;
    QHBoxLayout *maxLayBox;


signals:
    void max_sig_size(QGraphicsView *);
    void sig_clicked(QPointF);
};



#endif // MAXLAYOUT_H
