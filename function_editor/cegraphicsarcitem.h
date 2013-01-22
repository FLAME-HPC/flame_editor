#ifndef CEGRAPHICSELLIPSEITEM_H
#define CEGRAPHICSELLIPSEITEM_H

#include <QtGui>

class CEGraphicsArcItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 16 };
    explicit CEGraphicsArcItem(QPointF p1, QPointF p2, QGraphicsItem *parent = 0);
    
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    int type() const { return Type;}

    QPointF getStart() const {return start;}
    void setStart(QPointF p) { start = p;compute();}
    QPointF getStop() const {return stop;}
    void setStop(QPointF p) { stop = p;compute();}

private:
    QPainterPath myPath;
    QPainterPath headPath;
    QPointF start;
    QPointF stop;
    void compute();
    double Angle(QPointF o, QPointF p);
    double Distance(QPointF p1, QPointF p2);
    QPointF PointOnArc(QRectF rectangleF, double angle);
};

const double PI = 3.14159;

#endif // GRAPHICSELLIPSEITEM_H
