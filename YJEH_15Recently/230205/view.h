#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>


class View : public QGraphicsView
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);

    bool _pan;
    int _panStartX, _panStartY;


    QPointF clickPoint;


protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // VIEW_H
