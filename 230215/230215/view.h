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


protected:
    void mousePressEvent(QMouseEvent *event) override;  // 마우스를 클릭했을 때 실행하는 함수 override
    void mouseReleaseEvent(QMouseEvent *event) override;    // 마우스를 눌렀다가 뗐을 때 실행하는 함수 override
    void mouseMoveEvent(QMouseEvent *event) override;   // 마우스를 누르고 움직였을 때 실행하는 함수 override

};

#endif // VIEW_H
