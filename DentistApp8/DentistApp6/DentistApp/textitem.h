#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QObject>
#include <QGraphicsTextItem>

class QGraphicsSceneMouseEvent;

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    TextItem(QGraphicsItem *parent = nullptr);

signals:
    void lostFocus(TextItem* item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TEXTITEM_H
