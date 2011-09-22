#ifndef STATEITEM_H
#define STATEITEM_H

#include <QGraphicsItem>
#include <QObject>

class Arrow;
//#include "arrow.h"

class StateItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    enum { Type = UserType + 15 };
    StateItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QString getName();
    void setName(QString n);
    QPolygonF polygon() const
        { return myPolygon; }
    int type() const
        { return Type;}
    void addArrow(Arrow *arrow);
    void setStartState(bool ss);
    bool getStartState();
    QList<Arrow *> getTransitions() const { return arrows; }
    //int numOutGoingTransitions();

private:
    QPolygonF myPolygon;
    QRectF myRect;
    QFont * sansFont;
    QString name;
    QList<Arrow *> arrows;
    int nameWidth;
    int nameHeight;
    bool startState;
};

#endif // STATEITEM_H
