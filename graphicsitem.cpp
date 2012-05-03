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
    /* Set GraphicsItem flags */
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    /* Set font used for name */
    sansFont = new QFont("Helvetica", 12);
}

QString GraphicsItem::getName() {
    return name;
}

void GraphicsItem::setName(QString n) {
    /* Calculated the graphics item size dependent on the item name */
    name = n;
    /* Use font metrics */
    QFontMetrics fm(*sansFont);
    nameWidth = fm.width(name) + 10;
    nameHeight = fm.height();
    /* Calculate rectangle around the name text */
    if (mytype == 2) {  // message
        myNameRect = QRectF(-(nameWidth/2.0)-5.0,
                static_cast<float>(-nameHeight),
                static_cast<float>(nameWidth+10.0), nameHeight*2.0);
        /* For message type calculate the parallelogram shape line path */
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
    /* Update the shape bounding box for the graphics item */
    setBoundingRect();
}

void GraphicsItem::setState(QString name) {
    mytype = 0;
    setName(name);
}

void GraphicsItem::setTransition(QString name) {
    mytype = 1;
    setName(name);
}

void GraphicsItem::setMessage(QString name) {
    mytype = 2;
    setName(name);
}

void GraphicsItem::setBoundingRect() {
    prepareGeometryChange();
    myBoundingRect = myNameRect;
}

/** \fn GraphicsItem::boundingRect() const
 *  \brief The bounding rect where all painting must be restricted to. Used to determine if the item needs redrawing.
 *  \return The bounding rect.
 */
QRectF GraphicsItem::boundingRect() const {
    qreal adjust = 0.5;
    return myBoundingRect.adjusted(-adjust, -adjust, adjust, adjust);
}

/** \fn GraphicsItem::shape() const
 *  \brief The shape of the object used for collision detection, hit tests etc.
 *  \return The shape.
 */
QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    if (mytype == 0) path.addEllipse(myNameRect);
    if (mytype == 1) path.addRect(myNameRect);
    if (mytype == 2) return myPath;
    return path;
}

int GraphicsItem::width() {
    return myBoundingRect.width();
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
        QWidget *) {
    /* Set paint options */
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::white);
    painter->setPen(isSelected() ? Qt::red : Qt::black);
    /* Draw shape */
    if (mytype == 0) painter->drawEllipse(myNameRect);
    if (mytype == 1) painter->drawRect(myNameRect);
    if (mytype == 2) painter->drawPath(myPath);
    /* Draw text */
    painter->setFont(*sansFont);
    painter->drawText(-(nameWidth/2)+5, (nameHeight/2)-2, getName());
}
