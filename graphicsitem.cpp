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

GraphicsItem::GraphicsItem(ItemType ItemType, QString n, QGraphicsItem *parent, QGraphicsScene *scene)
    : mytype(ItemType) ,QGraphicsItem(parent, scene) {
    /* Set GraphicsItem flags */
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    /* Set font used for name */
    sansFont = new QFont("Helvetica", 12);

    if(n != "")
        setName(n);

    conditionGapName = 3;

    diamond = false;
    color = (QColor)Qt::black;

    switch (mytype)
    {
        case ConditionIf: conditionName = "if"; break;
        case ConditionWhileFor: conditionName = "while / for"; break;
        case ConditionWhile: conditionName = "while"; break;
        case ConditionFor: conditionName = "for"; break;
    }

    if(mytype == EmptyState || mytype == SolidState)
    {
        qreal g = 5;
        myNameRect = QRectF(-g, -g, g*2, g*2);
        setBoundingRect();
    }
}

GraphicsItem::~GraphicsItem()
{
}

void GraphicsItem::changeCondition(ItemType item)
{
    if(mytype == ConditionWhileFor || mytype == ConditionWhile || mytype == ConditionFor)
    {
        if(item == ConditionWhileFor || item == ConditionWhile || item == ConditionFor)
        {
            mytype = item;
            assignmentList.clear();
            if(mytype == ConditionWhileFor)
                conditionName = "while / for";
            else if(mytype == ConditionWhile)
            {
                conditionName = "while";
                assignmentList.append("");
            }
            else if(mytype == ConditionFor)
            {
                conditionName = "for";
                assignmentList.append("");
                assignmentList.append("");
                assignmentList.append("");
            }
        }
    }
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
    if (mytype == Message) {  // message
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
    }
    else
    if (mytype == ConditionIf || mytype == ConditionWhile || mytype == ConditionFor || mytype == ConditionWhileFor) {
        nameWidth = fm.width(name) + 10 >= fm.width(conditionName) + 10?fm.width(name) + 10:fm.width(conditionName) + 10;
        nameHeight = nameHeight + conditionGapName * 2;
        float mx = 20.0;
        float my = 20.0;
        myNameRect = QRectF(-(nameWidth/2.0) - mx, static_cast<float>(-nameHeight - my),
                static_cast<float>(nameWidth + mx * 2.0), nameHeight*2.0 + my * 2.0);
        /* For diamond type calculate the parallelogram shape line path */
        float mdx = myNameRect.width() / 2.0;
        float mdy = myNameRect.height() / 2.0;
        myPath = QPainterPath();
        myPath.moveTo(myNameRect.x(), myNameRect.y() + mdy);
        myPath.lineTo(myNameRect.x() + mdx, myNameRect.y());
        myPath.lineTo(myNameRect.topRight().x(), myNameRect.y() + mdy);
        myPath.lineTo(myNameRect.x() + mdx, myNameRect.bottomRight().y());
        myPath.lineTo(myNameRect.x(), myNameRect.y() + mdy);
    }
    else
    {
        myNameRect = QRectF(-nameWidth/2.0, static_cast<float>(-nameHeight),
                static_cast<float>(nameWidth), nameHeight*2.0);
    }
    /* Update the shape bounding box for the graphics item */
    setBoundingRect();
}

void GraphicsItem::setState(QString name) {
    mytype = State;
    setName(name);
}

void GraphicsItem::setTransition(QString name) {
    mytype = Transition;
    setName(name);
}

void GraphicsItem::setMessage(QString name) {
    mytype = Message;
    setName(name);
}

void GraphicsItem::setDiamond(QString name)
{
    mytype = ConditionIf;
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
    if (mytype == State || mytype == FinalState) path.addEllipse(myNameRect);
    if (mytype == Transition) path.addRect(myNameRect);
    if (mytype == Message) return myPath;
    if (mytype == ConditionIf || mytype == ConditionWhile || mytype == ConditionFor || mytype == ConditionWhileFor) return myPath;
    if (mytype == EmptyState) path.addEllipse(myNameRect);
    if (mytype == SolidState) path.addEllipse(myNameRect);
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
    painter->setPen(isSelected() ? Qt::red : color);
    /* Draw shape */
    if (mytype == State) painter->drawEllipse(myNameRect);
    if (mytype == Transition) painter->drawRect(myNameRect);
    if (mytype == Message) painter->drawPath(myPath);
    if (mytype == ConditionIf || mytype == ConditionWhile || mytype == ConditionFor || mytype == ConditionWhileFor) painter->drawPath(myPath);
    if (mytype == EmptyState || mytype == SolidState)
    {
        qreal g = 5;
        myNameRect = QRectF(-g, -g, g*2, g*2);
        //qDebug()<<"draw"<<myNameRect;
        QPen p;
        p.setWidth(2);
        p.setColor(isSelected() ? Qt::red : color);
        if(mytype == SolidState)
            painter->setBrush(isSelected() ? Qt::red : color);
        painter->setPen(p);
        painter->drawEllipse(myNameRect);
    }
    else if(mytype == FinalState)
    {
        QPen p;
        p.setWidth(3);
        p.setColor(isSelected() ? Qt::red : color);
        painter->setPen(p);
        painter->drawEllipse(myNameRect);
    }
    /* Draw text */

    if(mytype == State || mytype == FinalState || mytype == Transition || mytype == Message)
    {
        painter->setFont(*sansFont);
        painter->drawText(-(nameWidth/2)+5, (nameHeight/2)-2, getName());
    }
    else if(mytype == ConditionIf || mytype == ConditionWhile || mytype == ConditionFor || mytype == ConditionWhileFor)
    {
        QFontMetrics fm(*sansFont);
        nameHeight = fm.height();
        sansFont->setItalic(true);
        painter->setFont(*sansFont);
        nameWidth = fm.width(conditionName) + 10;
        painter->drawText(-(nameWidth/2)+5, -nameHeight/2, conditionName);
        sansFont->setItalic(false);
        painter->setFont(*sansFont);
        nameWidth = fm.width(name) + 10;
        painter->drawText(-(nameWidth/2)+5, (nameHeight/2)-2, getName());
    }
}
