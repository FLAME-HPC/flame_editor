/*!
 * \file graphicsitem.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of graphics item for stategraphs
 */
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include "./graphicsitem.h"

GraphicsItem::GraphicsItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    sansFont = new QFont("Helvetica", 12);
    foreign = false;
    editable = true;
    currentState_ = 0;
    nextState_ = 0;
    highlighted_ = false;
}

void GraphicsItem::setState(State *s) {
    myState = s;
    mytype = 0;
    setName(s->name());
}

void GraphicsItem::setTransition(Transition *t) {
    transition = t;
    mytype = 1;
    setName(t->name());
    setCondition(t->condition());
    preState = t->currentState()->name();
    postState = t->nextState()->name();
}

void GraphicsItem::setMessage(QString name) {
    mytype = 2;
    setName(name);
}

QString GraphicsItem::getName() {
    return name;
}

void GraphicsItem::nameChanged() {
    if (mytype == 0) setName(myState->name());
    else if (mytype == 1) setName(transition->name());
    else if (mytype == 2) setName(message->messageType);
}

void GraphicsItem::setName(QString n) {
    name = n;
    QFontMetrics fm(*sansFont);
    nameWidth = fm.width(name) + 10;
    nameHeight = fm.height();
    // prepareGeometryChange();
    if (mytype == 2) {  // message
        myNameRect = QRectF(-(nameWidth/2.0)-5.0,
                static_cast<float>(-nameHeight),
                static_cast<float>(nameWidth+10.0), nameHeight*2.0);
        myPath = QPainterPath();
        myPath.moveTo(myNameRect.bottomLeft());
        myPath.lineTo(myNameRect.bottomRight().x()-10,
                myNameRect.bottomRight().y());
        myPath.lineTo(myNameRect.topRight());
        myPath.lineTo(myNameRect.topLeft().x()+10, myNameRect.topLeft().y());
        myPath.lineTo(myNameRect.bottomLeft());
    } else {
        myNameRect = QRectF(-nameWidth/2.0, static_cast<float>(-nameHeight),
                static_cast<float>(nameWidth), nameHeight*2.0);
    }
    setBoundingRect();
}

void GraphicsItem::setCondition(Condition c) {
    condition = c;
    QString cond = c.toString();
    QFontMetrics fm(*sansFont);
    condWidth = fm.width(cond) + 10;
    condHeight = fm.height();
    myConditionRect = QRectF(-condWidth/2.0,
            static_cast<float>(-(condHeight*2)-nameHeight),
            static_cast<float>(condWidth), condHeight*2.0);
    setBoundingRect();
}

void GraphicsItem::addTransitionArrow(Arrow *arrow) {
    transitionArrows.append(arrow);
}

void GraphicsItem::addMessageArrow(Arrow *arrow) {
    messageArrows.append(arrow);
}

void GraphicsItem::removeMessageArrow(Arrow *arrow) {
    messageArrows.removeOne(arrow);
}

void GraphicsItem::removeTransitionArrow(Arrow *arrow) {
    transitionArrows.removeOne(arrow);
}

void GraphicsItem::setBoundingRect() {
    prepareGeometryChange();
    if (condition.enabled) {
        myBoundingRect = QRectF(qMin(myNameRect.x(),
            myConditionRect.x()), myConditionRect.y(),
            qMax(myNameRect.width(), myConditionRect.width()),
myNameRect.height()+myConditionRect.height());
    } else {
        myBoundingRect = myNameRect;
    }
}

/** \fn GraphicsItem::boundingRect() const
 *  \brief The bounding rect where all painting must be restricted to. Used to determine if the item needs redrawing.
 *  \return The bounding rect.
 */
QRectF GraphicsItem::boundingRect() const {
    qreal adjust = 0.5;
    // return QRectF(-20 - adjust, -10 - adjust, 40 + adjust, 20 + adjust);
    return myBoundingRect.adjusted(-adjust, -adjust, adjust, adjust);
}

/** \fn GraphicsItem::shape() const
 *  \brief The shape of the object used for collision detection, hit tests etc.
 *  \return The shape.
 */
QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    // path.addRect(-20, -10, 40, 20);
    // path.addRect(myRect);
    if (mytype == 0) path.addEllipse(myNameRect);
    if (mytype == 1) path.addRect(myNameRect);
    if (mytype == 2) return myPath;
    return path;
}

int GraphicsItem::width() {
    return myBoundingRect.width();
}

void GraphicsItem::setHighlighted(bool b) {
    // Update only if changed
    if (b != highlighted_) {
        highlighted_ = b;
        update();
    }
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
        QWidget *) {
    // Body
    // painter->setBrush(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);
    // if(startState) painter->setBrush(Qt::black);
    // else
    painter->setBrush(Qt::white);
    if (foreign) painter->setPen(Qt::gray);
    else
        painter->setPen((isSelected()||highlighted_)&&editable ? Qt::red : Qt::black);
    if (mytype == 2) painter->setPen(
                (isSelected()||highlighted_)&&editable ? Qt::darkGreen : Qt::green);
    // painter->drawEllipse(-20, -10, 40, 20);
    if (mytype == 0) painter->drawEllipse(myNameRect);
    if (mytype == 1) painter->drawRect(myNameRect);
    if (mytype == 2) painter->drawPath(myPath);

    // QFont sansFont("Helvetica", fontSize);

    painter->setFont(*sansFont);
    painter->drawText(-(nameWidth/2)+5, (nameHeight/2)-2, getName());
    if (condition.enabled) painter->drawText(-(condWidth/2)+5,
            (condHeight/2)-2 - nameHeight - condHeight, condition.toString());

    // Draw bounding box (for development)
    /*painter->setPen(Qt::DashLine);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(myBoundingRect);*/
}
