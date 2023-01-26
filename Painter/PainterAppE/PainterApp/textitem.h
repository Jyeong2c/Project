#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QObject>
#include <QGraphicsTextItem>

class QGraphicsSceneMouseEvent;

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 3};
    TextItem(QGraphicsItem *parent = nullptr);

    int type() const override {return Type;}
    ~TextItem();

signals:
    void lostFocus(TextItem* item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TEXTITEM_H
