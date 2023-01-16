#include "widget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    m_nPTargetPixmap = QPixmap(400, 400);
    m_nPTargetPixmap.fill();
    m_nbMousePressed = false;
}

Widget::~Widget()
{

}

void Widget::mousePressEvent(QMouseEvent *event){
    m_nbMousePressed = true;
    m_line.setP1(event->pos());
    m_line.setP2(event->pos());
}

void Widget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    m_nbMousePressed = false;
    update();
}

void Widget::paintEvent(QPaintEvent* /*e*/)
{
    static bool wasPressed = false;
    QPainter painter(this);

    if(m_nbMousePressed)
    {
        painter.drawPixmap(0, 0, m_nPTargetPixmap);
        painter.drawLine(m_line);
        wasPressed = true;
    }else if(wasPressed){
        QPainter PixmapPainter(&m_nPTargetPixmap);
        QPen pen(Qt::green);
        PixmapPainter.setPen(pen);
        PixmapPainter.drawLine(m_line);

        painter.drawPixmap(0, 0, m_nPTargetPixmap);
        wasPressed = false;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseMove)
    {
        m_line.setP2(event->pos());
    }
    update();
}
