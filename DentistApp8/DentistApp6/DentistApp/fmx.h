#ifndef FMX_H
#define FMX_H

#include <QWidget>

class FMX : public QWidget
{
    Q_OBJECT
public:
    explicit FMX(QWidget *parent = nullptr);

private:
    void fmx();

signals:

};

#endif // FMX_H
