/*!
 * \file graphicsitem.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for graphics items in stategraphs
 */
#ifndef GRAPHICSITEM_H_
#define GRAPHICSITEM_H_

#include <QGraphicsItem>
#include <QObject>
#include "./state.h"
#include "./transition.h"
#include "./messagecomm.h"
//#include "machine.h"

class Arrow;
class Machine;

class GraphicsItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    #if QT_VERSION >= 0x040600  // If Qt version is 4.6 or higher
    Q_INTERFACES(QGraphicsItem)
    #endif

  public:
    enum { Type = UserType + 15 };
    GraphicsItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QString getName();
    void nameChanged();
    void setCondition(Condition c);
    QPolygonF polygon() const
        { return myPolygon; }
    int type() const
        { return Type;}
    void addTransitionArrow(Arrow *arrow);
    void addMessageArrow(Arrow *arrow);
    void removeMessageArrow(Arrow * arrow);
    void removeTransitionArrow(Arrow * arrow);
    void setState(State * s);
    State * state() { return myState; }
    void setTransition(Transition * t);
    void setMessage(QString name);
    QList<Arrow *> getTransitionArrows() const { return transitionArrows; }
    QList<Arrow *> getMessageArrows() const { return messageArrows; }
    int width();
    void setHighlighted(bool b);
    int layer;
    int layered;
    bool foreign;
    bool editable;
    int aveX;
    int mytype;  // 0-state, 1-transition, 2-message
    QString agentName;
    QString preState;
    QString postState;
    Transition * transition;
    MessageComm * message;
    Machine * machine;
    GraphicsItem * currentState_;
    GraphicsItem * nextState_;
    void setName(QString n);

  private:
    void setBoundingRect();
    QPolygonF myPolygon;
    QRectF myNameRect;
    QRectF myConditionRect;
    QRectF myBoundingRect;
    QFont * sansFont;
    QString name;
    QList<Arrow *> transitionArrows;
    QList<Arrow *> messageArrows;
    int nameWidth;
    int nameHeight;
    int condWidth;
    int condHeight;
    State * myState;
    QPainterPath myPath;
    Condition condition;
    bool highlighted_;
};

#endif  // GRAPHICSITEM_H_
