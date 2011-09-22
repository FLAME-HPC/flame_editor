#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "stateitem.h"
#include "mpre.h"
#include "mpost.h"
#include "memorymodel.h"

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(StateItem *startItem, StateItem *endItem,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    StateItem *startItem() const
        { return myStartItem; }
    StateItem *endItem() const
        { return myEndItem; }
    QString getName() const
        { return name; }
    void setName(QString n);
    Mpre getMpre() const
    { return mpre; }
    void setMpre(Mpre m);
    Mpost getMpost() const
    { return mpost; }
    Mpre * getMprePointer() { return &mpre; }
    void setMpost(Mpost mp);
    //bool passesCondition(MemoryModel * memory);
    //void updateMemory(MemoryModel * memory);

public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    QString name;
    StateItem *myStartItem;
    StateItem *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
    Mpre mpre;
    Mpost mpost;
};

#endif // ARROW_H
