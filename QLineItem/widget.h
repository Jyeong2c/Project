#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QPixmap>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent* evnet) override;

private:
    QPixmap m_nPTargetPixmap;
    bool m_nbMousePressed;

    QLine m_line;
};
#endif // WIDGET_H
