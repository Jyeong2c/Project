#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);


private:
    void gridLayout();

signals:

};

#endif // LAYOUT_H
