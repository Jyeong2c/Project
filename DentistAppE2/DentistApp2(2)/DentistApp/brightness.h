#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <QObject>
#include <QWidget>

class Brightness : public QWidget
{
    Q_OBJECT
public:
    explicit Brightness(QWidget *parent = nullptr);

public slots:
    void bright();
    void contrast();
signals:

private:



};

#endif // BRIGHTNESS_H
