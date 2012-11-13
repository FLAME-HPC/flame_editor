/*!
 * \file arrow.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for arrow used in stategraphs
 */
#ifndef FEARROW_H_
#define FEARROW_H_

#include <QGraphicsLineItem>
#include "./fegraphicsitem.h"
//#include "./mpre.h"
//#include "./mpost.h"
//#include "./memorymodel.h"
//#include "./transition.h"

/*!
 * \brief Graphics arrow class
 *
 * This class represents an arrow in the stategraph graphics scene.
 */
class FEArrow : public QGraphicsLineItem {
  public:
    /*!
     * \brief This enum is used to hold a unique int associated with graphicsitems
     */
    enum {
        Type = UserType + 4 /**< enum Type is the unique graphicsitems int. */
    };

    FEArrow(FEGraphicsItem *getStartItem, FEGraphicsItem *getEndItem,
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
    FEGraphicsItem *getStartItem() const
        { return myStartItem; }
    FEGraphicsItem *getEndItem() const
        { return myEndItem; }
    void drawHead(bool b) { showHead = b; }

    void setStartItem(FEGraphicsItem *g) {myStartItem = g;}
    void setEndItem(FEGraphicsItem *g) {myEndItem = g;}

  public slots:
    void updatePosition();

  protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

  private:
    FEGraphicsItem *myStartItem;  /**< \brief the tail graphics item. */
    FEGraphicsItem *myEndItem;    /**< \brief the head graphics item. */
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

#endif  // FEARROW_H_
