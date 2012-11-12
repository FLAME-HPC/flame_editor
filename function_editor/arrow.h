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

    Arrow(GraphicsItem *getStartItem, GraphicsItem *getEndItem,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    QString getName() const { return name; }
    void setName(QString n);
    QRectF setBoundingRect();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    GraphicsItem *getStartItem() const
        { return myStartItem; }
    GraphicsItem *getEndItem() const
        { return myEndItem; }
    void drawHead(bool b) { showHead = b; }

    void setStartItem(GraphicsItem *g) {myStartItem = g;}
    void setEndItem(GraphicsItem *g) {myEndItem = g;}

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
    QRectF myBoundingRect;
    QString name;
    bool showName;
    QRectF myNameRect;
    QFont * sansFont;
    int nameWidth;
    int nameHeight;
};

#endif  // ARROW_H_
