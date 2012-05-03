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

class Arrow;

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
    void setName(QString n);
    QString getName();
    QPolygonF polygon() const { return myPolygon; }
    int type() const { return Type;}
    void setState(QString name);
    void setTransition(QString name);
    void setMessage(QString name);
    int width();
    int mytype;  /* 0-state, 1-transition, 2-message */

  private:
    void setBoundingRect();
    QPolygonF myPolygon;
    QRectF myNameRect;
    QRectF myConditionRect;
    QRectF myBoundingRect;
    QFont * sansFont;
    QString name;
    int nameWidth;
    int nameHeight;
    int condWidth;
    int condHeight;
    QPainterPath myPath;
};

#endif  // GRAPHICSITEM_H_
