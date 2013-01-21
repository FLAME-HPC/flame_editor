/*!
 * \file graphicsitem.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for graphics items in stategraphs
 */
#ifndef CEGRAPHICSITEM_H_
#define CEGRAPHICSITEM_H_

#include <QGraphicsItem>
#include <QObject>

class CEArrow;

enum CEItemType{
    State,
    Transition,
    Message,
    ConditionIf,
    ConditionWhile,
    ConditionFor,
    ConditionWhileFor,
    EmptyState,
    SolidState,
    FinalState
};

class CEGraphicsItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    #if QT_VERSION >= 0x040600  // If Qt version is 4.6 or higher
    Q_INTERFACES(QGraphicsItem)
    #endif

  public:
    enum { Type = UserType + 15 };
    CEGraphicsItem(CEItemType CEItemType = State, QString n = "", QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~CEGraphicsItem();

    void changeCondition(CEItemType item);

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
    void setDiamond(QString name);
    int width();
    CEItemType mytype;  /* 0-state, 1-transition, 2-message, 3-diamond */

    QColor getColor() { return color;}
    void setColor(QColor c) {color = c;}

    void addGraphicsItem(QGraphicsItem *g)
      {graphicsItemList.append(g);}
    void removeGraphicsItem(QGraphicsItem *g)
      {graphicsItemList.removeOne(g);}
    int countGraphicsItem()
      {return graphicsItemList.count();}
    QGraphicsItem* getGraphicsItem()
      {return graphicsItemList.last();}
    QGraphicsItem* getGraphicsItem(int i)
      {
        if(i >= 0 && i < graphicsItemList.count())
            return graphicsItemList[i];
    }

    bool isDiamond() const {return diamond;}
    void setDiamond(bool b) { diamond = b;}

    int countGraphicsItemParents() const
      {
        return graphicsItemParents.count();
      }

    CEGraphicsItem* getGraphicsItemParents() const
      {
        if(graphicsItemParents.count() == 0)
            return 0;
        else
            return graphicsItemParents[0];
      }

    void setGraphicsItemParents(CEGraphicsItem * p)
      {
        if(graphicsItemParents.count() == 0)
            graphicsItemParents.append(p);
        else
            graphicsItemParents[0] = p;
      }

    void addGraphicsItemParents(CEGraphicsItem * p)
      {
        graphicsItemParents.append(p);
      }
    void removeGraphicsItemParents(CEGraphicsItem *g)
      {graphicsItemParents.removeOne(g);}

    QStringList *getAssignmentList(){
        return &assignmentList;
    }

  private:
    void setBoundingRect();
    QPolygonF myPolygon;
    QRectF myNameRect;
    QRectF myConditionRect;
    QRectF myBoundingRect;
    QFont * sansFont;
    QString name;
    QString conditionName;
    qreal conditionGapName;
    int nameWidth;
    int nameHeight;
    int condWidth;
    int condHeight;
    QPainterPath myPath;
    QList<QGraphicsItem*> graphicsItemList;
    bool diamond;
    QColor color;
    QList<CEGraphicsItem*> graphicsItemParents;

    QStringList assignmentList;
};

#endif  // GRAPHICSITEM_H_
