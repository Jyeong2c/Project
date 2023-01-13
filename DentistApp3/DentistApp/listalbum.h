#ifndef LISTALBUM_H
#define LISTALBUM_H

#include <QWidget>
#include <QSplitter>

class QListWidget;

class Listalbum : public QSplitter
{
    Q_OBJECT
public:
    explicit Listalbum(QWidget *parent = nullptr);


private:
    QListWidget *listWidget;


public slots:
    void loadImages();

signals:

};

#endif // LISTALBUM_H
