#ifndef DELETEIMAGE_H
#define DELETEIMAGE_H

#include <QObject>

class DeleteImage : public QObject
{
    Q_OBJECT
public:
    DeleteImage();
    void deleteImage(QString = "", QString = "", QString = "");
};

#endif // DELETEIMAGE_H
