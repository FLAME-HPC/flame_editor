/*!
 * \file arrow.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of graphics arrow in stategraphs
 */
#include <QtGui>
#include <math.h>
#include "./arrow.h"

const qreal Pi = 3.14; /* definition of Pi */

/*!
 * \brief Arrow constructor
 * \param[in] startItem The tail graphics item
 * \param[in] endItem The head graphics item
 * \param[in] parent Not required, is automatically initalised
 * \param[in] scene Not required, is automatically initalised
 *
 * This contructor takes starting and ending graphics items for an arrow to be drawn between them.
 */
Arrow::Arrow(GraphicsItem *startItem, GraphicsItem *endItem,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene) {
    /* The start and end graphics items */
    myStartItem = startItem;
    myEndItem = endItem;
    /* Set graphics item flags */
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    /* Set colour and pen */
    myColor = Qt::black;
    setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    /* Be default show arrow head */
    showHead = true;

    sansFont = new QFont("Helvetica", 12);
    showName = false;
    setName("");
}

void Arrow::setName(QString n)
{
    name = n;
    /* Use font metrics */
    QFontMetrics fm(*sansFont);
    nameWidth = fm.width(name) + 10;
    nameHeight = fm.height();
    myNameRect = QRectF(line().p1().x(), line().p1().y(),
                        static_cast<float>(nameWidth), nameHeight*2.0);
}

QRectF Arrow::setBoundingRect()
{

    QRectF r = QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
            .normalized();
    if(r.height() > myNameRect.height()){
        if(r.width() < myNameRect.width()){
            qreal d = (nameWidth - myNameRect.width()) / 2.0;
            r = QRectF(r.x()-d,r.y(),nameWidth,r.height());
        }
        showName = true;
    }
    else
        showName = false;
    QRectF rr = QRectF(r.x() + (r.width()-myNameRect.width()) / 2.0,
                                   r.y() + (r.height()-myNameRect.height()) / 2.0,
                       myNameRect.width(), myNameRect.height());
    return rr;
}

QRectF Arrow::boundingRect() const{
    qreal extra = ((pen().width() + 20));
    //qDebug()<<"ok";
    //updatePosition();
    QRectF r = QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
            .normalized();
    if(r.height() > myNameRect.height()){
        if(r.width() < myNameRect.width()){
            qreal d = (nameWidth - myNameRect.width()) / 2.0;
            r = QRectF(r.x()-d,r.y(),nameWidth,r.height());
        }
    }
    return r.adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    //if(showName)
    //QRectF r = setBoundingRect();
    //path.addRect(r);
    return path;
}

void Arrow::updatePosition() {
    QLineF linef(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(linef);

    //setBoundingRect();
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *) {
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    /* Set drawing pen */
    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    painter->setPen(isSelected() ? Qt::red : Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);

    /* Calculate the intersection point with the end item
     * so that the arrow head can be drawn in the correct position*/
    QPainterPath endPath = myEndItem->shape();
    float dif = 0.1;
    QPointF intersectionPoint;
    QPointF ip;
    QLineF centerLine2(myStartItem->scenePos(), myEndItem->scenePos());

    for (float i = 0; i < 1.0; i+=dif) {
        float e = i+dif;
        if (e > 0.99) e = 0;
        QLineF myLine(endPath.pointAtPercent(i), endPath.pointAtPercent(e));
        QLineF myLine2(myLine.x1()+centerLine2.x2(),
                        myLine.y1()+centerLine2.y2(),
                        myLine.x2()+centerLine2.x2(),
                        myLine.y2()+centerLine2.y2());
        int rc = centerLine2.intersect(myLine2, &ip);
        if (rc == QLineF::BoundedIntersection) {
            intersectionPoint = ip;
            break;
        }
    }
    /* Set the line between the intersection point and the start item */
    setLine(QLineF(intersectionPoint, myStartItem->pos()));

    /* Calculate the angle of the line with the horizontal */
    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    /* This section was to draw curved lines */
    QPainterPath path; /* The line */
    QPointF half(line().x1() + (line().dx()/2), line().y1() + (line().dy()/2));
    path.moveTo(line().p1());
    /* Used to draw curves instead of straight lines */
    double offset = 0.0;
    QPointF c2 = half + QPointF(sin(angle + Pi) * offset,
            cos(angle + Pi) * offset);
    path.quadTo(c2, line().p2());
    QLineF l(line().p1(), c2);
    double angle3 = ::acos(l.dx() / l.length());
    angle = angle3;
    if (l.dy() >= 0)
        angle = (Pi * 2) - angle;

    /* Calculate the positions of the ends of the arrow head */
    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) *
            arrowSize, cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) *
            arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

    /* The clear() function for QVector has a
     * compile warning under Qt 4.5 */
    #if QT_VERSION >= 0x040600  // If Qt version is 4.6 or higher
    arrowHead.clear();
    #else
    arrowHead = QPolygonF();
    #endif

    arrowHead << line().p1();
    if (showHead) arrowHead << arrowP1 << arrowP2;

    /* Draw the line and arrow head */
    painter->drawPath(path);
    painter->setBrush(isSelected() ? Qt::red : Qt::black);
    painter->drawPolygon(arrowHead);

    //qDebug()<<myBoundingRect<<" "<<myNameRect;

    /* Draw text */
    myNameRect = setBoundingRect();
    //if(showName)
    {
        painter->setFont(*sansFont);
        painter->drawText(myNameRect.x(), myNameRect.y() + myNameRect.height(), name);
    }
}
