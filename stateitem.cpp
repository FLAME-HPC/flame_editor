#include "stateitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

StateItem::StateItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    sansFont = new QFont("Helvetica", 12);
    setName("default");
    startState = false;

    myPolygon = QPolygonF(myRect);
}

void StateItem::setStartState(bool ss)
{
    startState = ss;
}

bool StateItem::getStartState()
{
    return startState;
}

QString StateItem::getName()
{
    return name;
}

void StateItem::setName(QString n)
{
    name = n;
    QFontMetrics fm(*sansFont);
    nameWidth = fm.width(name) + 10;
    nameHeight = fm.height();
    prepareGeometryChange();
    myRect = QRectF(-nameWidth/2.0, (float)-nameHeight, (float)nameWidth, nameHeight*2.0);
}

void StateItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

QRectF StateItem::boundingRect() const
{
    qreal adjust = 0.5;
    //return QRectF(-20 - adjust, -10 - adjust, 40 + adjust, 20 + adjust);
    return myRect.adjusted(-adjust, -adjust, adjust, adjust);
}

QPainterPath StateItem::shape() const
{
    QPainterPath path;
    //path.addRect(-20, -10, 40, 20);
    path.addRect(myRect);
    return path;
}

void StateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{


    // Body
    //painter->setBrush(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);
    //if(startState) painter->setBrush(Qt::black);
    //else
    painter->setBrush(Qt::white);
    painter->setPen(isSelected() ? Qt::red : Qt::black);
    //painter->drawEllipse(-20, -10, 40, 20);
    painter->drawEllipse(myRect);

    //QFont sansFont("Helvetica", fontSize);

    painter->setFont(*sansFont);
    painter->drawText(-(nameWidth/2)+5, (nameHeight/2)-2, name);
/*
    // Eyes
    painter->setBrush(Qt::white);
    painter->drawEllipse(-10, -17, 8, 8);
    painter->drawEllipse(2, -17, 8, 8);

    // Nose
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    // Pupils
    painter->drawEllipse(QRectF(-8.0, -17, 4, 4));
    painter->drawEllipse(QRectF(4.0, -17, 4, 4));

    // Ears
    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    // Tail
    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
    */
}

/*int StateItem::numOutGoingTransitions()
{
    int count = 0;
    for(int i = 0; i < arrows.size(); i ++)
    {
        Arrow * a = arrows.at(i);
        if(a->startItem() == this) count++;
    }
    return count;
}*/
