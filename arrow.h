/*!
 * \file arrow.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for arrow used in stategraphs
 */
#ifndef ARROW_H_
#define ARROW_H_

#include <QGraphicsLineItem>
#include "./graphicsitem.h"
//#include "./mpre.h"
//#include "./mpost.h"
//#include "./memorymodel.h"
//#include "./transition.h"

/*!
 * \brief Graphics arrow class
 *
 * This class represents an arrow in the stategraph graphics scene.
 */
class Arrow : public QGraphicsLineItem {
  public:
    /*!
     * \brief This enum is used to hold a unique int associated with graphicsitems
     */
    enum {
        Type = UserType + 4 /**< enum Type is the unique graphicsitems int. */
    };

    Arrow(GraphicsItem *startItem, GraphicsItem *endItem,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    GraphicsItem *startItem() const
        { return myStartItem; }
    GraphicsItem *endItem() const
        { return myEndItem; }
    void drawHead(bool b) { showHead = b; }

  public slots:
    void updatePosition();

  protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

  private:
    GraphicsItem *myStartItem;  /**< \brief the tail graphics item. */
    GraphicsItem *myEndItem;    /**< \brief the head graphics item. */
    QColor myColor;             /**< \brief the arrow colour. */
    /*! \brief the polygon representation of the arrow head. */
    QPolygonF arrowHead;
    bool showHead;              /**< \brief flag to show the arrow head. */
};

#endif  // ARROW_H_
